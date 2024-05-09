// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Tetris.hpp"
#include "SFMLDisplay.hpp"
#include "ClientConfig.hpp"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    // Seed the random number generator
    srand(time(nullptr));

    tetriq::Tetris tetris(12, 22);
    tetriq::SFMLDisplay display;
    sf::Clock clock;
    tetriq::ClientConfig config;

    if (!display.loadGame(tetris))
        return EXIT_FAILURE;
    while (true) {
        if (!display.draw(tetris))
            break;
        if (!display.handleEvents(tetris))
            break;
        if (clock.getElapsedTime() > sf::seconds(1.0 / 5.0)) {
            clock.restart();
            tetris.tick();
        }
    }
    return EXIT_SUCCESS;
}
