// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/GameActionPacket.hpp"
#include "GameAction.hpp"
#include "network/PacketId.hpp"

namespace tetriq {
    GameActionPacket::GameActionPacket()
    {}

    GameActionPacket::GameActionPacket(GameAction action)
        : _action(action)
    {}

    PacketId GameActionPacket::getId() const
    {
        return PacketId::C_GAME_ACTION;
    }

    GameAction GameActionPacket::getAction() const
    {
        return _action;
    }

    NetworkOStream &GameActionPacket::operator>>(NetworkOStream &ns) const
    {
        return _action >> ns;
    }

    NetworkIStream &GameActionPacket::operator<<(NetworkIStream &ns)
    {
        return _action << ns;
    }

    size_t GameActionPacket::getNetworkSize() const
    {
        return sizeof(GameAction);
    }
}
