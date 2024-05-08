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

#include <csignal>
#include <toml++/toml.hpp>
#include "Network.hpp"
#include "Logger.hpp"

extern bool should_exit;

namespace tetriq {
    class Server {
        public:
            // Server exceptions
            /**
             * @brief Base exception class for the server
             */
            class ServerException: public std::exception {
                public:
                    explicit ServerException(std::string message);

                    [[nodiscard]] const char *what() const noexcept override;

                private:
                    std::string _message;
            };

            /**
             * @brief Exception thrown when the server failed to initialize
             */
            class ServerInitException final: public ServerException {
                public:
                    explicit ServerInitException();
            };

            // Server methods
            /**
             * @brief Server constructor
             * @param ip IP address to bind the server to
             * @param port Port to bind the server to
             * @param out Output stream for normal logs, defaults to std::cout
             * @param err Output stream for error logs, defaults to std::cerr
             * @exception ServerInitException if the server failed to initialize
             */
            Server(std::string ip, std::string port,
                std::ostream &out = std::cout, std::ostream &err = std::cerr);
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
            /**
             * @brief Create the server host
             * @return true if the server host was successfully created, false otherwise
             */
            bool createHost();
            /**
             * @brief Listen for all enet events,
             * stop when _running is false
             */
            void listen();
            /**
             * @brief Handle a new client connection
             * @param event ENet event containing the new client connection
             * @return true if the new client was successfully handled, false otherwise
             */
            bool handleNewClient(ENetEvent &event);
            /**
            * @brief Handle a client disconnection
            * @param event ENet event containing the client disconnection
            */
            void handleClientDisconnect(ENetEvent &event);
            /**
            * @brief Handle a client packet
            * @param event ENet event containing the client packet
            */
            void handleClientPacket(ENetEvent &event);
            /**
            * @brief Handle None event (timeout)
            * @param event ENet event containing the None event
            */
            void handleNone(ENetEvent &event);

        private:
            std::string _ip;
            std::string _port;
            Logger _logger;
            ENetAddress _address;
            ENetHost *_server;

            bool _running {true};

            /**
             * @todo load these values from a configuration file
             * @todo release 2.0
             */
            uint64_t _max_clients {5};
            uint64_t _max_channels {2};
            uint64_t _max_outgoing_bandwidth {0};
            uint64_t _max_incoming_bandwidth {0};
            uint32_t _timeout {1000};
    };
}
