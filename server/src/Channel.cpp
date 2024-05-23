// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Channel.hpp"
#include "Logger.hpp"
#include "Player.hpp"
#include "Server.hpp"
#include "network/APacket.hpp"
#include <chrono>
#include <cstddef>
#include <cstdint>

namespace tetriq {
    Channel::Channel(Server &server)
        : _server(server)
        , _next_tick()
        , _game_started(false)
        , _players()
    {}

    bool Channel::hasGameStarted() const
    {
        return _game_started;
    }

    bool Channel::addPlayer(Player &player)
    {
        uint64_t player_id = player.getNetworkId();
        for (uint64_t id : _players) {
            if (player_id == id) {
                return false;
            }
        }
        _players.emplace_back(player.getNetworkId());
        return true;
    }

    void Channel::removePlayer(Player &player)
    {
        uint64_t player_id = player.getNetworkId();
        for (size_t i = 0; i < _players.size(); i++) {
            if (player_id == _players[i]) {
                _players[i] = _players.back();
                _players.pop_back();
                return;
            }
        }
    }

    const std::vector<uint64_t> &Channel::getPlayers() const
    {
        return _players;
    }

    void Channel::startGame()
    {
        LogLevel::DEBUG << "starting game" << std::endl;
        for (uint64_t id : _players) {
            Player &player = _server.getPlayerById(id);
            Tetris &game = player.getGame();
            game = Tetris(_server.getConfig().game.width, _server.getConfig().game.height);
            player.startGame(_server.getConfig().game);
        }
        _game_started = true;
    }

    void Channel::stopGame()
    {
        LogLevel::DEBUG << "game stopped" << std::endl;
        _game_started = false;
    }

    void Channel::tick()
    {
        if (!hasGameStarted()) {
            if (_players.size() > 1)
                startGame();
            return;
        }

        std::chrono::steady_clock::duration now =
            std::chrono::steady_clock::now().time_since_epoch();
        if (now < _next_tick)
            return;
        _next_tick = now + std::chrono::nanoseconds(1'000'000'000 / 3);

        if (_players.size() == 0) {
            stopGame();
            return;
        }

        bool game_over = true;
        for (uint64_t id : _players) {
            Player &player = _server.getPlayerById(id);
            player.tickGame();
            game_over &= player.isGameOver();
        }

        if (game_over)
            stopGame();
    }

    void Channel::broadcastPacket(const APacket &packet)
    {
        for (uint64_t id : _players) {
            _server.getPlayerById(id).sendPacket(packet);
        }
    }
}
