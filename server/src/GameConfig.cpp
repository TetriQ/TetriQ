// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "GameConfig.hpp"

namespace tetriq {
    GameConfig::GameConfig(toml::table &config)
    {
        ticks_per_second = config["ticks_per_second"].value<int64_t>().value_or(5);
        width = config["width"].value<int64_t>().value_or(12);
        height = config["height"].value<int64_t>().value_or(12);
    }
}
