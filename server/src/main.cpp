// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Server.hpp"
#include "Logger.hpp"

#include <csignal>
#include <iostream>

bool should_exit = false;

/**
 * @brief Main function of the server
 */
int main(void)
{
    signal(SIGINT, [](int) {
        if (should_exit)
            raise(SIGKILL);
        should_exit = true;
        tetriq::Logger::log(tetriq::LogLevel::INFO, "SIGINT received, stopping server");
    });
    try {
        tetriq::Server server("127.0.0.1", "4242");
        server.listen();
    } catch (const tetriq::Server::ServerException &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
