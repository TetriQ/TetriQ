// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/ConnectPacket.hpp"

namespace tetriq {
    ConnectPacket::ConnectPacket()
    {}

    ConnectPacket::ConnectPacket(uint64_t player_id, uint64_t game_width, uint64_t game_height)
        : _game_width(game_width)
        , _game_height(game_height)
        , _player_id(player_id)
    {}

    PacketId ConnectPacket::getId() const
    {
        return PacketId::S_CONNECT;
    }

    uint64_t ConnectPacket::getGameWidth() const
    {
        return _game_width;
    }

    uint64_t ConnectPacket::getGameHeight() const
    {
        return _game_height;
    }

    uint64_t ConnectPacket::getPlayerId() const
    {
        return _player_id;
    }

    NetworkOStream &ConnectPacket::operator>>(NetworkOStream &ns) const
    {
        _game_width >> ns;
        _game_height >> ns;
        _player_id >> ns;
        return ns;
    }

    NetworkIStream &ConnectPacket::operator<<(NetworkIStream &ns)
    {
        _game_width << ns;
        _game_height << ns;
        _player_id << ns;
        return ns;
    }

    size_t ConnectPacket::getNetworkSize() const
    {
        return sizeof(_player_id) + sizeof(_game_width) + sizeof(_game_height);
    }
}
