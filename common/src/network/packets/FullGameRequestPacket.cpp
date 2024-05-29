// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/FullGameRequestPacket.hpp"
#include "network/PacketId.hpp"

namespace tetriq {
    PacketId FullGameRequestPacket::getId() const
    {
        return PacketId::C_FULL_GAME_REQUEST;
    }

    NetworkOStream &FullGameRequestPacket::operator>>(NetworkOStream &os) const
    {
        return os;
    }

    NetworkIStream &FullGameRequestPacket::operator<<(NetworkIStream &os)
    {
        return os;
    }

    size_t FullGameRequestPacket::getNetworkSize() const
    {
        return 0;
    }
}
