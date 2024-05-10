// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Client.hpp"
#include "Tetris.hpp"
#include "SFMLDisplay.hpp"
#include "ClientConfig.hpp"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    // Seed the random number generator
    srand(time(nullptr));

    try {
        tetriq::Client client("localhost", "4242");
    } catch (const tetriq::Client::ClientInitException &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const tetriq::Client::ClientConnectionException &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
