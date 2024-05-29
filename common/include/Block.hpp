// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <array>

namespace tetriq {
    class Tetris;

    enum class BlockType : uint64_t {
        EMPTY,
        RED,
        BLUE,
        DARK_BLUE,
        ORANGE,
        YELLOW,
        GREEN,
        PURPLE,
        INDESTRUCTIBLE,
        PU_ADD_LINE,
        PU_CLEAR_SPECIAL_BLOCK,
        PU_CLEAR_LINE,
        PU_CLEAR_BLOCK_RANDOM,
        PU_GRAVITY,
        PU_NUKE_FIELD,
        PU_BLOCK_BOMB,
        PU_BLOCK_QUAKE,
        PU_SWITCH_FIELD,
    };

    using TetroShape = std::pair<BlockType, std::vector<std::vector<int>>>;
    using TetroRotation = std::vector<std::tuple<char, char>>;

    extern const std::map<BlockType, std::vector<TetroRotation>> BLOCK_ROTATIONS;

    struct WeightedPowerUp {
            BlockType powerUp;
            uint64_t weight;

            static BlockType getRandom();
    };
}
