// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "IDisplay.hpp"
#include "ITetris.hpp"
#include "RemoteTetris.hpp"
#include "ClientConfig.hpp"
#include "ViewerTetris.hpp"
#include "network/PacketHandler.hpp"
#include "network/packets/InitGamePacket.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <enet/enet.h>

namespace tetriq {
    class Client : public PacketHandler {
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
            Client(std::string ip, uint16_t port, IDisplay &display);

            /**
            * @brief Client destructor
            */
            ~Client();

            /**
             * @brief Starts the main loop
             */
            void loop();

        private:
            /**
            * @brief Initialize the client
            * @return True if the client was initialized successfully
            */
            bool init() const;

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

            bool handle(InitGamePacket &packet) override;

            const ClientConfig _config;

            /**
            * @brief The client username
            * @todo not implemented yet
            */
            std::string _username;
            const std::string _server_ip;
            const uint16_t _server_port;

            ENetAddress _address;
            ENetHost *_client;
            ENetPeer *_server;

            bool _game_started;
            std::unique_ptr<RemoteTetris> _game;
            std::vector<std::unique_ptr<ITetris>> _external_games;
            IDisplay &_display;
    };
}
