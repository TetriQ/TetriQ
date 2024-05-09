// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Utils.hpp"
#include "Block.hpp"
#include <cstdint>

namespace tetriq {
    class Tetromino {
        public:
            Tetromino();
            explicit Tetromino(BlockType &&type);
            ~Tetromino();

            [[nodiscard]] BlockType getType() const;

            [[nodiscard]] pos getPosition() const;
            [[nodiscard]] int getRotation() const;
            void setRotation(int rotation);
            void setPosition(pos position);
            const Rotation &getBlockRotation() const;
            [[nodiscard]] bool move(int x, int y, const Tetris &game);
            [[nodiscard]] bool rotate(const Tetris &game);
            void drop(Tetris &game);
            bool collides(const Tetris &game) const;
        private:
            pos _position;
            BlockType _type;
            uint64_t _rotation = 0;
    };
}
