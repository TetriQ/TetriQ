// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "ViewerTetris.hpp"
#include "Logger.hpp"
#include "Tetris.hpp"
#include "network/packets/FullGamePacket.hpp"
#include <cstdint>

namespace tetriq {
    ViewerTetris::ViewerTetris(size_t width, size_t height, uint64_t player_id)
        : Tetris(width, height)
        , _player_id(player_id)
    {}

    bool ViewerTetris::handle(FullGamePacket &packet)
    {
        if (packet.getPlayerId() != _player_id) // Packet is not for us
            return false;
        Tetris::operator=(packet.getGame());
        return true;
    }
}
