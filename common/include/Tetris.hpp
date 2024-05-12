// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Block.hpp"
#include "Tetromino.hpp"

#include <cstdint>
#include <vector>
#include <memory>

namespace tetriq {
    class Tetris final {
        public:
            Tetris(size_t width, size_t height);
            ~Tetris();

            [[nodiscard]] uint64_t getWidth() const;
            [[nodiscard]] uint64_t getHeight() const;

            const std::unique_ptr<Block> &getBlockAt(uint64_t x, uint64_t y) const;
            const Tetromino &getCurrentPiece() const;
            Tetromino &getCurrentPiece();
            const Tetromino &getNextPiece() const;

            [[nodiscard]] bool moveCurrentPiece(int xOffset, int yOffset);
            [[nodiscard]] bool rotateCurrentPiece();
            void dropCurrentPiece();

            /**
             * Advances the game by one tick.
             */
            void tick();
            void addGraceTicks(uint64_t n);

            /**
             * Returns true if the game is over.
             */
            bool isOver() const;
        private:
            void placeTetromino();

            /**
             * Prevents the block from being placed on the next n ticks. This is
             * used by dropCurrentPiece().
             */
            uint64_t _grace_ticks;
            bool _game_over;
            
            std::vector<std::vector<std::unique_ptr<Block>>> _blocks;
            std::vector<Tetromino> _nextPieces;
            uint64_t _width;
            uint64_t _height;
    };
}
