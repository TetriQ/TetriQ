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

#include <map>

#include "Block.hpp"
#include "Network.hpp"
#include <vector>
#include <memory>

namespace tetriq {
    class Tetris final: public NetworkObject {
        public:
            Tetris(size_t width, size_t height);
            ~Tetris() override;

            [[nodiscard]] uint64_t getWidth() const;
            [[nodiscard]] uint64_t getHeight() const;
            [[nodiscard]] const std::unique_ptr<Block> &getBlockAt(uint64_t x,
                uint64_t y) const;

        private:
            std::vector<std::vector<std::unique_ptr<Block>>> _blocks;
            uint64_t _width;
            uint64_t _height;
    };
}
