// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Client.hpp"
#include "IDisplay.hpp"
#include "Logger.hpp"
#include "SFMLDisplay.hpp"
#include "Arguments.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{
    tetriq::Arguments args;
    if (!args.parse(argc, argv)) {
        tetriq::LogLevel::INFO << "usage: ./tetriq_client ip port" << std::endl;
        return EXIT_FAILURE;
    }

    // Seed the random number generator
    srand(time(nullptr));

    try {
        std::unique_ptr<tetriq::IDisplay> display{new tetriq::SFMLDisplay{}};
        tetriq::Client client(args.ip, args.port, *display);
        client.loop();
    } catch (const tetriq::Client::ClientInitException &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const tetriq::Client::ClientConnectionException &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
