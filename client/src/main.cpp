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
    tetriq::Tetris tetris(10, 20);
    tetriq::SFMLDisplay display;

    if (!display.loadGame(tetris))
        return EXIT_FAILURE;
    while (display.draw(tetris));
    return EXIT_SUCCESS;
}
