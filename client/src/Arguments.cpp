// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Arguments.hpp"
#include "Logger.hpp"
#include <stdexcept>

namespace tetriq {
    bool Arguments::parse(int argc, char *argv[])
    {
        if (argc != 4) {
            LogLevel::ERROR << "Invalid number of arguments" << std::endl;
            return false;
        }
        ip = argv[1];
        try {
            int n = std::stoi(argv[2]);
            if ((uint16_t) n != n) {
                throw std::out_of_range("");
            }
            port = n;
        } catch (std::logic_error &) {
            LogLevel::ERROR << "Invalid port number" << std::endl;
            return false;
        }
        display = DisplayFactory::createFromName(argv[3]);
        if (!display) {
            LogLevel::ERROR << "Invalid display name" << std::endl;
            return false;
        }
        return true;
    }
}
