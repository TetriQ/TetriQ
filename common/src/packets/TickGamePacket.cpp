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
        : _width(game.getWidth())
        , _height(game.getHeight())
        , _nextPieces(game.getNextPieces())
    {}

    PacketId TickGamePacket::getId() const
    {
        return PacketId::S_TICK_GAME;
    }

    NetworkOStream &TickGamePacket::operator>>(NetworkOStream &ns) const
    {
        _width >> ns;
        _height >> ns;
        _nextPieces >> ns;
        return ns;
    }

    NetworkIStream &TickGamePacket::operator<<(NetworkIStream &ns)
    {
        _width << ns;
        _height << ns;
        _nextPieces << ns;
        return ns;
    }

    size_t TickGamePacket::getNetworkSize() const
    {
        return sizeof(uint64_t) * 3 + _nextPieces[0].getNetworkSize() * _nextPieces.size(); // TODO : not good
    }
}
