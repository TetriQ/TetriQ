// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Channel.hpp"
#include "Player.hpp"
#include "ServerConfig.hpp"
#include "rcon/Rcon.hpp"

#include <chrono>
#include <csignal>
#include <cstdint>
#include <toml++/toml.hpp>
#include <enet/enet.h>
#include <unordered_map>
#include <vector>

extern bool should_exit;

namespace tetriq {
    class Server {
        public:
            /**
             * @brief Base exception class for the server
             */
            class ServerException : public std::exception {
                public:
                    explicit ServerException(std::string message);

                    [[nodiscard]] const char *what() const noexcept override;

                private:
                    std::string _message;
            };

            /**
             * @brief Exception thrown when the server failed to initialize
             */
            class ServerInitException final : public ServerException {
                public:
                    explicit ServerInitException();
            };

            /**
             * @brief Server constructor
             * @exception ServerInitException if the server failed to initialize
             */
            Server();

            /**
             * @brief Server destructor
             */
            ~Server();

            /**
             * @brief Start main loop of listening to network events and managing players.
             */
            void listen();

            /**
             * @returns a player based on his network id.
             */
            Player &getPlayerById(uint64_t id);

            /**
             * @returns the server's config.
             */
            const ServerConfig &getConfig() const;

            std::vector<Channel> &getChannels();

            bool createChannel();
            bool deleteChannel(uint64_t id);

        private:
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
             * @brief Handle all rcon events.
             */
            bool handleRconEvents();

            /**
             * @brief Listen for all enet events.
             */
            bool handleENetEvents();

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
            void handleNone(ENetEvent &event) const;

            const ServerConfig _config;
            ENetAddress _address;
            ENetHost *_server;

            std::chrono::steady_clock::time_point _next_tick;
            bool _running{true};
            uint64_t _network_id_counter{0};
            std::unordered_map<uint64_t, Player> _players;
            uint64_t _channel_id_counter{0};
            std::vector<Channel> _channels;
            Rcon _rcon;
    };
}
