// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/TickGamePacket.hpp"
#include "Tetris.hpp"
#include "network/PacketId.hpp"
#include <cstdint>

namespace tetriq {
    TickGamePacket::TickGamePacket()
    {}

    TickGamePacket::TickGamePacket(uint64_t player_id, const Tetris &game)
        : _player_id(player_id)
        , _game(game)
    {}

    PacketId TickGamePacket::getId() const
    {
        return PacketId::S_TICK_GAME;
    }

    uint64_t TickGamePacket::getPlayerId() const
    {
        return _player_id;
    }

    const Tetris &TickGamePacket::getGame() const
    {
        return _game;
    }

    NetworkOStream &TickGamePacket::operator>>(NetworkOStream &ns) const
    {
        _player_id >> ns;
        _game >> ns;
        return ns;
    }

    NetworkIStream &TickGamePacket::operator<<(NetworkIStream &ns)
    {
        _player_id << ns;
        _game << ns;
        return ns;
    }

    size_t TickGamePacket::getNetworkSize() const
    {
        return sizeof(_player_id) + _game.getNetworkSize();
    }
}
