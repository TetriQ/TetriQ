// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Block.hpp"
#include <cstdint>

namespace tetriq {
    const std::map<BlockType, std::vector<TetroRotation>> BLOCK_ROTATIONS = {
        {BlockType::YELLOW,
            {
                {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            }},
        {BlockType::DARK_BLUE,
            {{{0, 0}, {1, 0}, {2, 0}, {2, 1}},
                {{1, -1}, {1, 0}, {1, 1}, {0, 1}},
                {{0, -1}, {0, 0}, {1, 0}, {2, 0}},
                {{1, -1}, {2, -1}, {1, 0}, {1, 1}}}},
        {BlockType::ORANGE,
            {{{0, 0}, {1, 0}, {2, 0}, {0, 1}},
                {{0, -1}, {1, -1}, {1, 0}, {1, 1}},
                {{2, -1}, {0, 0}, {1, 0}, {2, 0}},
                {{1, -1}, {1, 0}, {1, 1}, {2, 1}}}},
        {BlockType::RED, {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {{1, 0}, {0, 1}, {1, 1}, {0, 2}}}},
        {BlockType::GREEN, {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, {{0, 0}, {0, 1}, {1, 1}, {1, 2}}}},
        {BlockType::PURPLE,
            {{{1, 0}, {0, 1}, {1, 1}, {2, 1}},
                {{1, 0}, {1, 1}, {2, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {1, 2}},
                {{1, 0}, {0, 1}, {1, 1}, {1, 2}}}},
        {BlockType::BLUE, {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}, {{1, -1}, {1, 0}, {1, 1}, {1, 2}}}},
    };
}
