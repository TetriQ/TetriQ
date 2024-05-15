// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Block.hpp"
#include "GameAction.hpp"
#include "ITetris.hpp"
#include "Tetromino.hpp"
#include "network/NetworkObject.hpp"

#include <cstdint>
#include <vector>

namespace tetriq {
    class Tetris final : public ITetris, public NetworkObject {
        public:
            Tetris(size_t width, size_t height);
            ~Tetris();

            uint64_t getWidth() const override;
            uint64_t getHeight() const override;

            BlockType getBlockAt(uint64_t x, uint64_t y) const override;
            const Tetromino &getCurrentPiece() const override;
            const Tetromino &getNextPiece() const override;

            const std::vector<Tetromino> &getNextPieces() const;

            [[nodiscard]] bool moveCurrentPiece(int xOffset, int yOffset);
            [[nodiscard]] bool rotateCurrentPiece();
            void dropCurrentPiece();
            void handleGameAction(GameAction action) override;

            /**
             * Advances the game by one tick.
             */
            void tick();
            void addGraceTicks(uint64_t n);

            /**
             * Returns true if the game is over.
             */
            bool isOver() const;

            NetworkOStream &operator>>(NetworkOStream &os) const override;
            NetworkIStream &operator<<(NetworkIStream &os) override;
            size_t getNetworkSize() const override;
        private:
            bool isLineFull(uint64_t y) const;
            bool moveBlock(Position oldPos, Position newPos);
            void placeTetromino();
            Tetromino &getCurrentPiece();

            /**
             * Prevents the block from being placed on the next n ticks. This is
             * used by dropCurrentPiece().
             */
            uint64_t _grace_ticks;
            bool _game_over;

            uint64_t _width;
            uint64_t _height;
            std::vector<std::vector<BlockType>> _blocks;
            std::vector<Tetromino> _nextPieces;
    };
}
