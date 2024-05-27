// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/FullGamePacket.hpp"
#include "Tetris.hpp"
#include "network/PacketId.hpp"
#include <cstdint>

namespace tetriq {
    FullGamePacket::FullGamePacket()
    {}

    FullGamePacket::FullGamePacket(uint64_t player_id, const Tetris &game)
        : _player_id(player_id)
        , _game(game)
    {}

    PacketId FullGamePacket::getId() const
    {
        return PacketId::S_FULL_GAME;
    }

    uint64_t FullGamePacket::getPlayerId() const
    {
        return _player_id;
    }

    const Tetris &FullGamePacket::getGame() const
    {
        return _game;
    }

    NetworkOStream &FullGamePacket::operator>>(NetworkOStream &ns) const
    {
        _player_id >> ns;
        _game >> ns;
        return ns;
    }

    NetworkIStream &FullGamePacket::operator<<(NetworkIStream &ns)
    {
        _player_id << ns;
        _game << ns;
        return ns;
    }

    size_t FullGamePacket::getNetworkSize() const
    {
        return sizeof(_player_id) + _game.getNetworkSize();
    }
}
