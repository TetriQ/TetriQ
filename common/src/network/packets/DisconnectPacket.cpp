// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/DisconnectPacket.hpp"

namespace tetriq {
    DisconnectPacket::DisconnectPacket()
    {}

    DisconnectPacket::DisconnectPacket(uint64_t player_id)
        : _player_id(player_id)
    {}

    PacketId DisconnectPacket::getId() const
    {
        return PacketId::S_DISCONNECT;
    }

    uint64_t DisconnectPacket::getPlayerId() const
    {
        return _player_id;
    }

    NetworkOStream &DisconnectPacket::operator>>(NetworkOStream &ns) const
    {
        _player_id >> ns;
        return ns;
    }

    NetworkIStream &DisconnectPacket::operator<<(NetworkIStream &ns)
    {
        _player_id << ns;
        return ns;
    }

    size_t DisconnectPacket::getNetworkSize() const
    {
        return sizeof(_player_id);
    }
}
