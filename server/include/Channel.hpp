// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Player.hpp"
#include "network/APacket.hpp"
#include <chrono>
#include <cstdint>
#include <ctime>
#include <vector>

namespace tetriq {
    class Server;

    class Channel {
        public:
            Channel(Server &server);

            /**
             * @returns true if the game is started
             */
            bool hasGameStarted() const;

            /**
             * @returns false if the player is already in the channel
             */
            [[nodiscard]] bool addPlayer(Player &player);
            void removePlayer(Player &player);
            const std::vector<uint64_t> &getPlayers() const;

            /**
             * @brief Get a player by its id (call _server.getPlayerById(id))
             * @param id of the player
             * @return the player with the given id if it exists and is in the channel
             * @throw std::out_of_range if the player is not in the channel
             */
            Player &getPlayerById(uint64_t id);

            void startGame();
            void stopGame();
            void tick();

            void broadcastPacket(const APacket &packet);

        private:
            Server &_server;
            std::chrono::steady_clock::duration _next_tick;
            bool _game_started;
            std::vector<uint64_t> _players;
    };
}
