// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/TickGamePacket.hpp"
#include "network/NetworkStream.hpp"
#include "network/PacketId.hpp"
#include <cstddef>
#include <cstdint>

namespace tetriq {
    TickGamePacket::TickGamePacket()
    {}

    TickGamePacket::TickGamePacket(uint64_t applied_actions)
        : _applied_actions(applied_actions)
    {}

    PacketId TickGamePacket::getId() const
    {
        return PacketId::S_TICK_GAME;
    }

    uint64_t TickGamePacket::getAppliedActions() const
    {
        return _applied_actions;
    }

    NetworkOStream &TickGamePacket::operator>>(NetworkOStream &ns) const
    {
        _applied_actions >> ns;
        return ns;
    }

    NetworkIStream &TickGamePacket::operator<<(NetworkIStream &ns)
    {
        _applied_actions << ns;
        return ns;
    }

    size_t TickGamePacket::getNetworkSize() const
    {
        return sizeof(_applied_actions);
    }
}
