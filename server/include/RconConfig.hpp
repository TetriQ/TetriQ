// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "AConfig.hpp"

namespace tetriq {
    class RconConfig {
        public:
            RconConfig() = default;
            RconConfig(toml::table &config);

            bool enabled = false;
            std::string password = "root";
            std::string listen_address = "0.0.0.0";
            uint16_t listen_port = 31458;
    };
}
