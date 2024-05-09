// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

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
