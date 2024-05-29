// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Player.hpp"
#include "Channel.hpp"
#include "GameConfig.hpp"
#include "Logger.hpp"
#include "network/packets/FullGamePacket.hpp"
#include "network/packets/GameActionPacket.hpp"
#include "network/packets/InitGamePacket.hpp"
#include "network/packets/TickGamePacket.hpp"
#include <cassert>
#include <cstdint>
#include <string>

namespace tetriq {
    Player::Player(uint64_t network_id, ENetPeer *peer,
        Channel &channel)
        : _network_id(network_id)
        , _peer(peer)
        , _channel(channel)
        , _game(12, 22)
    {
        Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " connected.");
        // Network ids are unique so no way the player was already added
        assert(channel.addPlayer(*this));
    }

    Player::~Player()
    {
        _channel.removePlayer(*this);
        Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " disconnected.");
    }

    void Player::startGame(const GameConfig &config)
    {
        std::vector<uint64_t> other_players = _channel.getPlayers();
        other_players.erase(std::remove(other_players.begin(), other_players.end(), _network_id));
        InitGamePacket{config.width, config.height, _network_id, other_players}.send(_peer);
        FullGamePacket{_network_id, _game}.send(_peer);
    }

    void Player::tickGame()
    {
        _game.tick();
        FullGamePacket packet{_network_id, _game};
        _channel.broadcastPacket(packet);
    }

    uint64_t Player::getNetworkId() const
    {
        return _network_id;
    }

    void Player::sendPacket(const APacket &packet)
    {
        packet.send(_peer);
    }

    bool Player::handle(GameActionPacket &packet)
    {
        _game.handleGameAction(packet.getAction());
        return true;
    }

    bool Player::isGameOver() const
    {
        return _game.isOver();
    }

    Tetris &Player::getGame()
    {
        return _game;
    }
}
