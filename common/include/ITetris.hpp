// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Block.hpp"
#include "GameAction.hpp"
#include "Tetromino.hpp"

#include <deque>

namespace tetriq {
    class ITetris {
        public:
            virtual ~ITetris() = default;

            virtual uint64_t getWidth() const = 0;
            virtual uint64_t getHeight() const = 0;
            virtual BlockType getBlockAt(uint64_t x, uint64_t y) const = 0;
            virtual const Tetromino &getCurrentPiece() const = 0;
            virtual const Tetromino &getNextPiece() const = 0;
            virtual const std::deque<BlockType> &getPowerUps() const = 0;

            virtual bool handleGameAction(GameAction action) = 0;
    };
}
