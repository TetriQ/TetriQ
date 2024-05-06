/*
* This file is part of TetriQ.
* Copyright © 2024 Sébastien Lucas <sebastien.lucas@epitech.eu>
*
* TetriQ is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* TetriQ is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TetriQ. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "Network.hpp"

#include <vector>
#include <map>

namespace tetriq {
    class Tetris;

    enum BlockType {
        EMPTY,
        RED,
        BLUE,
        DARK_BLUE,
        ORANGE,
        YELLOW,
        GREEN,
        PURPLE,
        INDESTRUCTIBLE,
        SPECIAL // not used for now
    };

    inline std::map<BlockType, std::vector<std::vector<int>>> blockShapes = {
        {
            YELLOW, {
                {1, 1},
                {1, 1}
            }
        },
        {
            ORANGE, {
                {1, 1, 1},
                {1, 0, 0}
            }
        },
        {
            DARK_BLUE, {
                {1, 1, 1},
                {0, 0, 1}
            }
        },
        {
            PURPLE, {
                {1, 1, 1},
                {0, 1, 0}
            }
        },
        {
            BLUE, {
                {1, 1, 1, 1}
            }
        },
        {
            RED, {
                {1, 1, 0},
                {0, 1, 1}
            }
        },
        {
            GREEN, {
                {0, 1, 1},
                {1, 1, 0}
            }
        }
    };

    class Block: public NetworkObject {
        public:
            explicit Block(Tetris &tetris, BlockType type);
            ~Block() override = default;
            virtual bool isDestructible() = 0;
            virtual void onDestruction() = 0;
            [[nodiscard]] BlockType getType() const;

        private:
            Tetris &_tetris;
            BlockType _type;
            bool _isDestructible;
    };

    class StandardBlock final: public Block {
        public:
            explicit StandardBlock(Tetris &_tetris, BlockType type);
            ~StandardBlock() override;
            bool isDestructible() override;
            void onDestruction() override;
    };

    class IndestructibleBlock final: public Block {
        public:
            explicit IndestructibleBlock(Tetris &_tetris);
            ~IndestructibleBlock() override;
            bool isDestructible() override;
            void onDestruction() override;
    };

    class SpecialBlock: public Block {
        public:
            explicit SpecialBlock(Tetris &_tetris, BlockType type);
            ~SpecialBlock() override;
            bool isDestructible() override;
            void onDestruction() override;
    };
}
