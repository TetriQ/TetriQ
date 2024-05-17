// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/InitGamePacket.hpp"
#include "network/PacketId.hpp"
#include <cstdint>

namespace tetriq {
    InitGamePacket::InitGamePacket()
        : _game_width(0)
        , _game_height(0)
        , _player_ids()
    {}

    InitGamePacket::InitGamePacket(
        uint64_t game_width, uint64_t game_height, const std::vector<uint64_t> &player_ids)
        : _game_width(game_width)
        , _game_height(game_height)
        , _player_ids(player_ids)
    {}

    PacketId InitGamePacket::getId() const
    {
        return PacketId::S_INIT_GAME;
    }

    uint64_t InitGamePacket::getGameWidth() const
    {
        return _game_width;
    }

    uint64_t InitGamePacket::getGameHeight() const
    {
        return _game_height;
    }

    const std::vector<uint64_t> &InitGamePacket::getPlayerIds() const
    {
        return _player_ids;
    }

    NetworkOStream &InitGamePacket::operator>>(NetworkOStream &ns) const
    {
        _game_width >> ns;
        _game_height >> ns;
        _player_ids >> ns;
        return ns;
    }

    NetworkIStream &InitGamePacket::operator<<(NetworkIStream &ns)
    {
        _game_width << ns;
        _game_height << ns;
        _player_ids << ns;
        return ns;
    }

    size_t InitGamePacket::getNetworkSize() const
    {
        return sizeof(uint64_t) * (3 + _player_ids.size());
    }
}
