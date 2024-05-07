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

#include "Block.hpp"

tetriq::Block::Block(Tetris &tetris, BlockType type)
: _tetris(tetris), _type(type), _isDestructible(false)
{}

tetriq::BlockType tetriq::Block::getType() const
{
    return _type;
}

tetriq::StandardBlock::StandardBlock(Tetris &_tetris, BlockType type)
: Block(_tetris, type)
{}

tetriq::StandardBlock::~StandardBlock()
= default;

bool tetriq::StandardBlock::isDestructible()
{
    return true;
}

void tetriq::StandardBlock::onDestruction()
{}

tetriq::IndestructibleBlock::IndestructibleBlock(Tetris &_tetris)
: Block(_tetris, INDESTRUCTIBLE)
{}

tetriq::IndestructibleBlock::~IndestructibleBlock() = default;

bool tetriq::IndestructibleBlock::isDestructible()
{
    return false;
}

void tetriq::IndestructibleBlock::onDestruction()
{}

tetriq::SpecialBlock::SpecialBlock(Tetris &_tetris, BlockType type) : Block(
    _tetris, type)
{}

tetriq::SpecialBlock::~SpecialBlock() = default;

bool tetriq::SpecialBlock::isDestructible()
{
    return true;
}

void tetriq::SpecialBlock::onDestruction()
{}

namespace tetriq {
    const std::map<BlockType, std::vector<Rotation>> BLOCK_ROTATIONS = {
        {
            YELLOW, {
                {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            }
        },
        {
            DARK_BLUE, {
                {{0, 0}, {1, 0}, {2, 0}, {2, 1}},
                {{1, -1}, {1, 0}, {1, 1}, {0, 1}},
                {{0, -1}, {0, 0}, {1, 0}, {2, 0}},
                {{1, -1}, {2, -1}, {1, 0}, {1, 1}}
            }
        },
        {
            ORANGE, {
                {{0, 0}, {1, 0}, {2, 0}, {0, 1}},
                {{0, -1}, {1, -1}, {1, 0}, {1, 1}},
                {{2, -1}, {0, 0}, {1, 0}, {2, 0}},
                {{1, -1}, {1, 0}, {1, 1}, {2, 1}}
            }
        },
        {
            RED, {
                {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
                {{1, 0}, {0, 1}, {1, 1}, {0, 2}}
            }
        },
        {
            GREEN, {
                {{1, 0}, {2, 0}, {0, 1}, {1, 1}},
                {{0, 0}, {0, 1}, {1, 1}, {1, 2}}
            }
        },
        {
            PURPLE, {
                {{1, 0}, {0, 1}, {1, 1}, {2, 1}},
                {{1, 0}, {1, 1}, {2, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {1, 2}},
                {{1, 0}, {0, 1}, {1, 1}, {1, 2}}
            }
        },
        {
            BLUE, {
                {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
                {{1, -1}, {1, 0}, {1, 1}, {1, 2}}
            }
        },
    };
}
