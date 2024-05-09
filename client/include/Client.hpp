// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <string>

#include "Network.hpp"
#include "Logger.hpp"

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

            Client(std::string ip, std::string port);
            ~Client();

        private:
            bool init() const;
            void log(LogLevel level, const std::string &message) const;
            bool setServer();
            bool connectToServer();

            std::string _username;
            const std::string _server_ip;
            const std::string _server_port;

            ENetAddress _address;
            ENetHost *_client;
            ENetPeer *_server;

            /**
             * @todo load these values from a configuration file
             * @todo release 2.0
             */
            uint64_t _max_channels{2};
            uint64_t _max_outgoing_bandwidth{0};
            uint64_t _max_incoming_bandwidth{0};
            uint32_t _timeout{1000};

            bool _logging{true};
            Logger _logger;
    };
}
