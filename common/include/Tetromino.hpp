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
