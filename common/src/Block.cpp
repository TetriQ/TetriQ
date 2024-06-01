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

    static constexpr std::array<WeightedPowerUp, 9> powerUps = {{
        {BlockType::PU_ADD_LINE, 100},
        {BlockType::PU_CLEAR_SPECIAL_BLOCK, 100},
        {BlockType::PU_CLEAR_LINE, 100},
        {BlockType::PU_CLEAR_BLOCK_RANDOM, 100},
        {BlockType::PU_GRAVITY, 100},
        {BlockType::PU_NUKE_FIELD, 100},
        {BlockType::PU_BLOCK_BOMB, 1},
        {BlockType::PU_COLUMN_SHUFFLE, 100},
        {BlockType::PU_SWITCH_FIELD, 100},
    }};

    static constexpr uint64_t TOTAL_POWERUPS_WEIGHT = std::accumulate(powerUps.begin(),
        powerUps.end(), 0, [](uint64_t sum, const WeightedPowerUp &pu) { return sum + pu.weight; });

    BlockType WeightedPowerUp::getRandom()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<uint64_t> dis(0, TOTAL_POWERUPS_WEIGHT - 1);

        uint64_t r = dis(gen);
        for (const auto &pu : powerUps) {
            if (r < pu.weight) {
                return pu.powerUp;
            }
            r -= pu.weight;
        }
        return BlockType::PU_ADD_LINE;
    }
}
