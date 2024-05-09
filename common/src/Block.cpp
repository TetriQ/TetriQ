// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

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
