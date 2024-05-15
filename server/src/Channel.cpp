#include "Channel.hpp"
#include "Logger.hpp"
#include "Player.hpp"
#include "Server.hpp"
#include <chrono>
#include <cstddef>
#include <cstdint>

namespace tetriq {
    Channel::Channel()
        : _next_tick()
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

    void Channel::startGame(Server &server)
    {
        for (uint64_t id : _players) {
            Player &player = server.getPlayerById(id);
            player.startGame();
        }
        _game_started = true;
    }
    
    void Channel::tick(Server &server)
    {
        if (!hasGameStarted()) {
            startGame(server);
            return;
        }
        std::chrono::steady_clock::duration now = std::chrono::steady_clock::now().time_since_epoch();
        if (now < _next_tick)
            return;
        _next_tick = now + std::chrono::seconds(1);

        for (uint64_t id : _players) {
            Player &player = server.getPlayerById(id);
            player.tickGame();
        }
    }
}
