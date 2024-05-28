// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Arguments.hpp"
#include "Logger.hpp"
#include <stdexcept>

namespace tetriq {
    bool Arguments::parse(int argc, char *argv[])
    {
        if (argc != 3) {
            LogLevel::ERROR << "invalid number of arguments" << std::endl;
            return false;
        }
        ip = argv[1];
        try {
            int n = std::stoi(argv[2]);
            if ((uint16_t) n != n) {
                throw std::out_of_range("");
            }
            port = n;
        } catch (std::logic_error) {
            LogLevel::ERROR << "invalid port number" << std::endl;
            return false;
        }
        return true;
    }
}
