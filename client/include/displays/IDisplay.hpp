// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Client.hpp"
#include "ITetris.hpp"
#include <Tetris.hpp>

namespace tetriq {
    class IDisplay {
        public:
            enum class TabType {
                GAME,
                CHAT,
                SCOREBOARD,
                HELP,
            };

            virtual ~IDisplay() = default;

            using ITetrisIter = std::vector<std::unique_ptr<ViewerTetris>>::const_iterator;

            [[nodiscard]] virtual bool loadGame(const ITetris &game, uint64_t player_count) = 0;
            [[nodiscard]] virtual bool draw(
                const Client &Client, ITetrisIter otherGamesStart, ITetrisIter otherGamesEnd) = 0;
            [[nodiscard]] virtual bool handleEvents(Client &Client) = 0;
    };
}
