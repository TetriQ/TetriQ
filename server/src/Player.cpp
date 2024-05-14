// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Player.hpp"
#include "Logger.hpp"
#include "network/IPacket.hpp"
#include "network/NetworkStream.hpp"
#include "network/TestPacket.hpp"
#include "network/packets/TickGamePacket.hpp"
#include <cstdint>
#include <string>

tetriq::Player::Player(uint64_t network_id, ENetPeer *peer)
    : _network_id(network_id)
    , _peer(peer)
    , _game(12, 22)
{
    Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " connected.");
}

tetriq::Player::~Player()
{
    Logger::log(LogLevel::INFO, "player " + std::to_string(_network_id) + " disconnected.");
}

void tetriq::Player::tickGame()
{
    _game.tick();
    sendPacket(TickGamePacket{_game});
}

void tetriq::Player::sendPacket(const tetriq::IPacket &packet) const
{
    NetworkOStream stream{sizeof(uint64_t) + packet.getNetworkSize()};

    static_cast<uint64_t>(packet.getId()) >> stream;
    packet >> stream;

    ENetPacket *epacket =
        enet_packet_create(stream.getData(), stream.getSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(_peer, 0, epacket);
}

uint64_t tetriq::Player::getNetworkId() const
{
    return _network_id;
}
