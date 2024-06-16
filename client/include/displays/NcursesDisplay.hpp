// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "IDisplay.hpp"
#include "ITetris.hpp"

#include <ncurses.h>
#include <cstdint>

namespace tetriq {
    class NcursesDisplay : public IDisplay {
        public:
            NcursesDisplay();
            ~NcursesDisplay() override;

            bool loadGame(const ITetris &game, uint64_t player_count) override;
            bool draw(const Client &client, ITetrisIter otherGamesStart,
                ITetrisIter otherGamesEnd) override;
            bool handleEvents(Client &client) override;

        private:
            void drawGame(
                const ITetris &game, Position position, uint64_t block_size, bool is_target);
            void drawBlock(Position pos, BlockType block, uint64_t block_size, bool is_target,
                short colorOverride = ERR, char charOverride = ERR);
            void drawTetromino(const Tetromino &tetromino, Position position, uint64_t block_size,
                short colorOverride = ERR, char charOverride = ERR);
            void drawCurrentTetromino(const ITetris &game);
            void drawNextTetromino(const ITetris &game);
            void drawPrediction(ITetris &game);
            void drawPowerUps(const ITetris &game);
            void rescaleGame();
            void displayHelp();

            const uint64_t BLOCK_SIZE = 1;
            const uint64_t SIDEBAR_SIZE = 6;

            WINDOW *_game_window;
            WINDOW *_menu_window;
            int _scr_width;
            int _scr_height;

            bool _show_help = false;
    };
}
