// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/TickGamePacket.hpp"
#include "Tetris.hpp"
#include "network/PacketId.hpp"

namespace tetriq {
    TickGamePacket::TickGamePacket()
    {}

    TickGamePacket::TickGamePacket(const Tetris &game)
    {
        _game = game;
    }

    PacketId TickGamePacket::getId() const
    {
        return PacketId::S_TICK_GAME;
    }

    const Tetris &TickGamePacket::getGame() const
    {
        return _game;
    }

    NetworkOStream &TickGamePacket::operator>>(NetworkOStream &ns) const
    {
        _game >> ns;
        return ns;
    }

    NetworkIStream &TickGamePacket::operator<<(NetworkIStream &ns)
    {
        _game << ns;
        return ns;
    }

    size_t TickGamePacket::getNetworkSize() const
    {
        return _game.getNetworkSize();
    }
}
