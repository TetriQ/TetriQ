// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Player.hpp"
#include "Channel.hpp"
#include "Logger.hpp"
#include "network/packets/GameActionPacket.hpp"
#include "network/packets/InitGamePacket.hpp"
#include "network/packets/TickGamePacket.hpp"
#include <cassert>
#include <cstdint>
#include <string>

tetriq::Player::Player(uint64_t network_id, ENetPeer *peer, Channel *channel)
    : _network_id(network_id)
    , _peer(peer)
    , _channel(channel)
    , _game(12, 22)
{
    Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " connected.");
    // Network ids are unique so no way the player was already added
    assert(channel->addPlayer(*this));
}

tetriq::Player::~Player()
{
    _channel->removePlayer(*this);
    Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " disconnected.");
}

void tetriq::Player::startGame()
{
    // TODO : actual game config and player ids
    InitGamePacket{12, 22, {}}.send(_peer);
}

void tetriq::Player::tickGame()
{
    _game.tick();
    TickGamePacket{_game}.send(_peer);
}

uint64_t tetriq::Player::getNetworkId() const
{
    return _network_id;
}

bool tetriq::Player::handle(tetriq::GameActionPacket &packet)
{
    _game.handleGameAction(packet.getAction());
    return true;
}
