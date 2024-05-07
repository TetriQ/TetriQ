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

#include "Tetris.hpp"
#include "SFMLDisplay.hpp"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    // Seed the random number generator
    srand(time(nullptr));

    tetriq::Tetris tetris(12, 22);
    tetriq::SFMLDisplay display;
    sf::Clock clock;

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
