// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Block.hpp"

tetriq::Block::Block(BlockType type)
: _type(type), _isDestructible(false)
{}

tetriq::BlockType tetriq::Block::getType() const
{
    return _type;
}

tetriq::StandardBlock::StandardBlock(BlockType type)
: Block(type)
{}

tetriq::StandardBlock::~StandardBlock()
= default;

bool tetriq::StandardBlock::isDestructible()
{
    return true;
}

void tetriq::StandardBlock::onDestruction()
{}

tetriq::IndestructibleBlock::IndestructibleBlock()
    : Block(BlockType::INDESTRUCTIBLE)
{}

tetriq::IndestructibleBlock::~IndestructibleBlock() = default;

bool tetriq::IndestructibleBlock::isDestructible()
{
    return false;
}

void tetriq::IndestructibleBlock::onDestruction()
{}

tetriq::SpecialBlock::SpecialBlock(BlockType type)
    : Block(type)
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
            BlockType::YELLOW, {
                {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            }
        },
        {
            BlockType::DARK_BLUE, {
                {{0, 0}, {1, 0}, {2, 0}, {2, 1}},
                {{1, -1}, {1, 0}, {1, 1}, {0, 1}},
                {{0, -1}, {0, 0}, {1, 0}, {2, 0}},
                {{1, -1}, {2, -1}, {1, 0}, {1, 1}}
            }
        },
        {
            BlockType::ORANGE, {
                {{0, 0}, {1, 0}, {2, 0}, {0, 1}},
                {{0, -1}, {1, -1}, {1, 0}, {1, 1}},
                {{2, -1}, {0, 0}, {1, 0}, {2, 0}},
                {{1, -1}, {1, 0}, {1, 1}, {2, 1}}
            }
        },
        {
            BlockType::RED, {
                {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
                {{1, 0}, {0, 1}, {1, 1}, {0, 2}}
            }
        },
        {
            BlockType::GREEN, {
                {{1, 0}, {2, 0}, {0, 1}, {1, 1}},
                {{0, 0}, {0, 1}, {1, 1}, {1, 2}}
            }
        },
        {
            BlockType::PURPLE, {
                {{1, 0}, {0, 1}, {1, 1}, {2, 1}},
                {{1, 0}, {1, 1}, {2, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {1, 2}},
                {{1, 0}, {0, 1}, {1, 1}, {1, 2}}
            }
        },
        {
            BlockType::BLUE, {
                {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
                {{1, -1}, {1, 0}, {1, 1}, {1, 2}}
            }
        },
    };
}
