// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Tetris.hpp"
#include "network/IPacket.hpp"
#include <enet/enet.h>
#include <cstdint>

namespace tetriq {
    class Player {
        public:
            Player(uint64_t network_id, ENetPeer *peer);
            ~Player();

            void sendPacket(const IPacket &packet) const;
            
            void tickGame();

            uint64_t getNetworkId() const;
        private:
            const uint64_t _network_id;
            ENetPeer *const _peer;

            Tetris _game;
    };
}
