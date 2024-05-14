// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>
#include <vector>
#include <map>

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
        SPECIAL // not used for now
    };

    using TetroShape = std::pair<BlockType, std::vector<std::vector<int>>>;
    using TetroRotation = std::vector<std::tuple<char, char>>;

    extern const std::map<BlockType, std::vector<TetroRotation>> BLOCK_ROTATIONS;
}
