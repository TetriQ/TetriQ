// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "ITetris.hpp"
#include <Tetris.hpp>

namespace tetriq {
    class IDisplay {
        public:
            virtual ~IDisplay() = default;

            using ITetrisIter = std::vector<std::unique_ptr<ITetris>>::const_iterator;
            
            [[nodiscard]] virtual bool loadGame(const ITetris &game, uint64_t player_count) = 0;
            [[nodiscard]] virtual bool draw(const ITetris &game, ITetrisIter otherGamesStart, ITetrisIter otherGamesEnd) = 0;
            [[nodiscard]] virtual bool handleEvents(ITetris &game) = 0;
    };
}
