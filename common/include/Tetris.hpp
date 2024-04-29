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

#ifndef TETRIS_H
#define TETRIS_H

#include "Network.hpp"
#include <vector>
#include <memory>

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

    class Block: public NetworkObject {
        public:
            explicit Block(Tetris &tetris, BlockType type);
            ~Block() override = default;
            virtual bool isDestructible() = 0;
            virtual void onDestruction() = 0;
            void fromString(const std::string &string) override;
            std::string toString() override;

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

    class Tetris final: public NetworkObject {
        public:
            Tetris(size_t width, size_t height);
            ~Tetris() override;
            std::string toString() override;
            void fromString(const std::string &string) override;

        private:
            std::vector<std::vector<std::shared_ptr<Block>>> _blocks;
            size_t _width;
            size_t _height;
    };
}

#endif //TETRIS_H
