// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <string>

#include "Network.hpp"
#include "Logger.hpp"
#include "SFMLDisplay.hpp"
#include "Tetris.hpp"
#include "ClientConfig.hpp"

namespace tetriq {
    class Client {
        public:
            class ClientException: public std::exception {
                public:
                    explicit ClientException(std::string message);

                    [[nodiscard]] const char *what() const noexcept override;

                private:
                    std::string _message;
            };

            class ClientInitException final: public ClientException {
                public:
                    explicit ClientInitException();
            };

            class ClientConnectionException final: public ClientException {
                public:
                    explicit ClientConnectionException();
            };

            /**
            * @brief Client constructor
            * @param ip The server IP address
            * @param port The server port
            */
            Client(std::string ip, std::string port);

            /**
            * @brief Client destructor
            */
            ~Client();

        private:
            /**
            * @brief Initialize the client
            * @return True if the client was initialized successfully
            */
            bool init() const;

            /**
            * @brief Log a message
            * @param level The log level
            * @param message The message to log
            */
            void log(LogLevel level, const std::string &message) const;

            /**
            * @brief Set the server address
            * @return True if the server address was set successfully
            */
            bool setServer();

            /**
            * @brief Connect to the server
            * @return True if the client connected to the server successfully
            */
            bool connectToServer();

            const ClientConfig _config;

            /**
            * @brief The client username
            * @todo not implemented yet
            */
            std::string _username;
            const std::string _server_ip;
            const std::string _server_port;

            ENetAddress _address;
            ENetHost *_client;
            ENetPeer *_server;

            /**
            * @todo remove this and replace logs it with static methods calls
            */
            bool _logging{true};
            Logger _logger;

            Tetris _tetris {12, 22};
            SFMLDisplay _display;
    };
}
