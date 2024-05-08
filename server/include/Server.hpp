/*
* This file is part of TetriQ.
* Copyright © 2024 Sébastien Lucas <sebastien.lucas@epitech.eu>
*
* TetriQ is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* TetriQ is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TetriQ. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "Network.hpp"
#include "Logger.hpp"

namespace tetriq {
    class Server {
        public:
            // Server exceptions
            class ServerException: public std::exception {
                public:
                    explicit ServerException(std::string message);

                    [[nodiscard]] const char *what() const noexcept override;

                private:
                    std::string _message;
            };

            class ServerInitException final: public ServerException {
                public:
                    explicit ServerInitException();
            };

            // Server methods
            /**
             * @brief Server constructor
             * @param ip IP address to bind the server to
             * @param port Port to bind the server to
             * @param logfile_name Name of the log file default is "tetriq_server"
             * @exception ServerInitException if the server failed to initialize
             */
            Server(std::string ip, std::string port,
                const std::string &logfile_name = "tetriq_server");
            /**
             * @brief Server destructor
             */
            ~Server();
            /**
             * @brief Initialize the server
             * @return true if the server was successfully initialized, false otherwise
             */
            bool init();
            /**
             * @brief Set the host address and port
             * @return true if the host address and port were successfully set, false otherwise
             */
            bool setHost();

        private:
            std::string _ip;
            std::string _port;
            Logger _logger;
            ENetAddress _address;
            ENetHost *_server;
    };
}