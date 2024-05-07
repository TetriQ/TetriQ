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

#include <Tetris.hpp>

namespace tetriq {
    class IDisplay {
        public:
            virtual ~IDisplay() = default;

            [[nodiscard]] virtual bool loadGame(const Tetris &game) = 0;
            [[nodiscard]] virtual bool draw(const Tetris &game) = 0;
            [[nodiscard]] virtual bool handleEvents(Tetris &game) = 0;
    };
}
