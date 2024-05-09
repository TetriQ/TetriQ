// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "IDisplay.hpp"

#include <SFML/Graphics.hpp>

namespace tetriq {
    class SFMLDisplay: public IDisplay {
        public:
            SFMLDisplay();
            ~SFMLDisplay() override;

            bool loadGame(const Tetris &game) override;
            bool draw(const Tetris &game) override;
            bool handleEvents(Tetris &game) override;
        private:
            void drawBlock(sf::Vector2u pos, BlockType block);
            void drawTetromino(const Tetromino &tetromino, Position position);
            void drawCurrentTetromino(const Tetris &game);
            void drawNextTetromino(const Tetris &game);

            const uint64_t BLOCK_SIZE = 32;
            const uint64_t SIDEBAR_SIZE = 6;

            sf::RenderWindow _window;
            sf::Event _event;
    };
}
