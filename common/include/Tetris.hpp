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
#include <deque>
#include <vector>

namespace tetriq {
    class Tetris : public ITetris, public NetworkObject {
        public:
            Tetris(size_t width, size_t height);
            ~Tetris();

            uint64_t getWidth() const override;
            uint64_t getHeight() const override;

            BlockType getBlockAt(uint64_t x, uint64_t y) const override;
            const Tetromino &getCurrentPiece() const override;
            const Tetromino &getNextPiece() const override;

            const std::vector<Tetromino> &getNextPieces() const;
            const std::deque<BlockType> &getPowerUps() const override;

            [[nodiscard]] bool moveCurrentPiece(int xOffset, int yOffset);
            [[nodiscard]] bool rotateCurrentPiece();
            void dropCurrentPiece();
            bool handleGameAction(GameAction action) override;
            BlockType consumePowerUp();
            void applyPowerUp(BlockType powerUp);
            void clearLine(uint64_t y);
            static void createBorders(
                std::vector<std::vector<BlockType>> &_board, uint64_t _width, uint64_t _height);
            bool isChanged();
            void setPowerUps(const std::deque<BlockType> &powerUps);
            void setChanged(bool changed);
            void setGameOver(bool game_over);

            /**
             * Advances the game by one tick.
             * @returns true if a block was placed.
             */
            void tick();
            void addGraceTicks(uint64_t n);
            uint64_t getCurrentTick() const;

            /**
             * Returns true if the game is over.
             */
            bool isOver() const;

            NetworkOStream &operator>>(NetworkOStream &os) const override;
            NetworkIStream &operator<<(NetworkIStream &os) override;
            size_t getNetworkSize() const override;

        private:
            void doPuAddLine();
            void doPuClearLine();
            void doPuClearSpecialBlock();
            void doPuClearBlockRandom();
            void doPuGravity();
            void doPuNukeField();
            void doPuColumnShuffle();

            void moveBlocksDown(uint64_t y);
            void moveBlocksUp(uint64_t y);

            bool isLineFull(uint64_t y) const;

            /**
             *
             * @return The number of blocks(!= EMPTY / INDESTRUCTIBLE) on the board.
             */
            uint64_t countBlocks() const;
            std::vector<Position> getBlocks() const;
            bool moveBlock(Position oldPos, Position newPos);
            void placeTetromino();
            Tetromino &getCurrentPiece();

            /**
             * @brief Removes all full lines from the board.
             * @param changed reference to a boolean that will be set to true if any lines were
             * removed.
             * @param lines_deleted reference to an unsigned int that will be incremented by the
             * number of lines removed.
             * @return void
             */
            void removeLinesFulls(bool &changed, unsigned int &lines_deleted);
            uint64_t getMaxHeight() const;

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
            std::deque<BlockType> _powerUps;

            uint64_t _tick{0};
            bool _changed{false};
    };
}
