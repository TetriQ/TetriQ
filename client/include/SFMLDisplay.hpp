/*
 * This file is part of TetriQ.
 * Copyright © 2024 Noé Lopez <noelopez@free.fr>
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

#include "IDisplay.hpp"

#include <SFML/Graphics.hpp>

namespace tetriq {
    class SFMLDisplay : public IDisplay {
        public:
            SFMLDisplay();
            ~SFMLDisplay();

            bool loadGame(const Tetris &game) override;
            bool draw(const Tetris &game) override;
            void drawBlock(sf::Vector2u pos, BlockType block);
        private:
            const uint64_t BLOCK_SIZE = 24;

            sf::RenderWindow _window;
    };
}
