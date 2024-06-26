// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "DisplayFactory.hpp"

namespace tetriq {
    class Arguments {
        public:
            bool parse(int argc, char *argv[]);

            std::string ip;
            uint16_t port;
            std::unique_ptr<IDisplay> display;
    };
}
