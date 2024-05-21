// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Client.hpp"
#include "IDisplay.hpp"
#include "SFMLDisplay.hpp"
#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    // Seed the random number generator
    srand(time(nullptr));

    try {
        std::unique_ptr<tetriq::IDisplay> display{new tetriq::SFMLDisplay{}};
        tetriq::Client client("localhost", 31457, *display);
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
