// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <Tetris.hpp>

namespace tetriq {
    class IDisplay {
        public:
            virtual ~IDisplay() = default;

            [[nodiscard]] virtual bool loadGame(const ITetris &game) = 0;
            [[nodiscard]] virtual bool draw(const ITetris &game) = 0;
            [[nodiscard]] virtual bool handleEvents(ITetris &game) = 0;
    };
}
