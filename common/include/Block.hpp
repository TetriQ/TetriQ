// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

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

    using BlockShape = std::pair<BlockType, std::vector<std::vector<int>>>;
    using Rotation = std::vector<std::tuple<char, char>>;

    extern const std::map<BlockType, std::vector<Rotation>> BLOCK_ROTATIONS;

    class Block {
        public:
            explicit Block(Tetris &tetris, BlockType type);
            virtual ~Block() = default;
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
