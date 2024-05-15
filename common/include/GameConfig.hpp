// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>
#include <toml++/toml.hpp>

namespace tetriq {
    class GameConfig {
        public:
            GameConfig() = default;
            GameConfig(toml::table &config);

            uint32_t ticks_per_second = 5;
            uint32_t width = 12;
            uint32_t height = 22;
    };
}
