// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Tetris.hpp"
#include "network/PacketHandler.hpp"
#include <cstdint>

namespace tetriq {
    /**
     * Class for viewing read-only games of other players.
     */
    class ViewerTetris : public Tetris, public PacketHandler {
        public:
            ViewerTetris(size_t width, size_t height, uint64_t player_id);

            bool handle(TickGamePacket &packet) override;
        private:
            uint64_t _player_id;
    };
}
