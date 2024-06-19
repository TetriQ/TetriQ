// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Client.hpp"
#include "Arguments.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
    tetriq::Arguments args;
    if (!args.parse(argc, argv)) {
        std::cerr << "USAGE: ./tetriq_client <ip> <port> <displayMode>\n\n"
                  << "  ip\t\tIP address of the server\n"
                  << "  port\t\tPort of the server\n"
                  << "  displayMode\tDisplay mode ('sfml' or 'ncurses')\n\n"
                  << "This program is free software: you can redistribute it and/or modify\n"
                  << "it under the terms of the GNU Affero General Public License as\n"
                  << "published by the Free Software Foundation, either version 3 of the\n"
                  << "License, or (at your option) any later version.\n\n"
                  << "Full documentation: <https://tetriq.noé.eu/>" << std::endl;
        return EXIT_FAILURE;
    }

    // Seed the random number generator
    srand(time(nullptr));

    try {
        tetriq::Client client(args.ip, args.port, std::move(args.display));
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
