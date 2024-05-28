// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/TickGamePacket.hpp"
#include "GameAction.hpp"
#include "network/NetworkStream.hpp"
#include "network/PacketId.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace tetriq {
    TickGamePacket::TickGamePacket()
    {}

    PacketId TickGamePacket::getId() const
    {
        return PacketId::S_TICK_GAME;
    }

    uint64_t TickGamePacket::getPlayerId() const
    {
        return _player_id;
    }

    const std::vector<GameAction> &TickGamePacket::getActions() const
    {
        return _actions;
    }

    NetworkOStream &TickGamePacket::operator>>(NetworkOStream &ns) const
    {
        _player_id >> ns;
        _actions >> ns;
        return ns;
    }

    NetworkIStream &TickGamePacket::operator<<(NetworkIStream &ns)
    {
        _player_id << ns;
        _actions << ns;
        return ns;
    }

    size_t TickGamePacket::getNetworkSize() const
    {
        return sizeof(_player_id) + sizeof(uint64_t) + sizeof(uint64_t) * _actions.size();
    }
}