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

    FullGamePacket::FullGamePacket(uint64_t player_id, const Tetris &game, uint64_t applied_actions)
        : _player_id(player_id)
        , _game(game)
        , _applied_actions(applied_actions)
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

    uint64_t FullGamePacket::getAppliedActions() const
    {
        return _applied_actions;
    }

    NetworkOStream &FullGamePacket::operator>>(NetworkOStream &ns) const
    {
        _player_id >> ns;
        _game >> ns;
        _applied_actions >> ns;
        return ns;
    }

    NetworkIStream &FullGamePacket::operator<<(NetworkIStream &ns)
    {
        _player_id << ns;
        _game << ns;
        _applied_actions << ns;
        return ns;
    }

    size_t FullGamePacket::getNetworkSize() const
    {
        return sizeof(_player_id) + sizeof(_applied_actions) + _game.getNetworkSize();
    }
}
