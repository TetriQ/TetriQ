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

    using BlockShape = std::pair<BlockType, std::vector<std::vector<int>>>;
    using Rotation = std::vector<std::tuple<char, char>>;

    extern const std::map<BlockType, std::vector<Rotation>> BLOCK_ROTATIONS;

    class Block {
        public:
            explicit Block(BlockType type);
            virtual ~Block() = default;
            virtual bool isDestructible() = 0;
            virtual void onDestruction() = 0;
            [[nodiscard]] BlockType getType() const;

        private:
            BlockType _type;
            bool _isDestructible;
    };

    class StandardBlock final: public Block {
        public:
            explicit StandardBlock(BlockType type);
            ~StandardBlock() override;
            bool isDestructible() override;
            void onDestruction() override;
    };

    class IndestructibleBlock final: public Block {
        public:
            explicit IndestructibleBlock();
            ~IndestructibleBlock() override;
            bool isDestructible() override;
            void onDestruction() override;
    };

    class SpecialBlock: public Block {
        public:
            explicit SpecialBlock(BlockType type);
            ~SpecialBlock() override;
            bool isDestructible() override;
            void onDestruction() override;
    };
}
