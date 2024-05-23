// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "GameAction.hpp"
#include "ITetris.hpp"
#include "Tetris.hpp"
#include "network/PacketHandler.hpp"
#include "network/packets/TickGamePacket.hpp"
#include <cstdint>

namespace tetriq {
    /**
     * @brief A tetris game that is controlled by a remote server.
     */
    class RemoteTetris : public ITetris, public PacketHandler {
        public:
            RemoteTetris(size_t width, size_t height, ENetPeer *peer, uint64_t player_id);

            void handleGameAction(GameAction action) override;
            uint64_t getWidth() const override;
            uint64_t getHeight() const override;
            BlockType getBlockAt(uint64_t x, uint64_t y) const override;
            const Tetromino &getCurrentPiece() const override;
            const Tetromino &getNextPiece() const override;
        private:
            bool handle(TestPacket &packet) override;
            bool handle(TickGamePacket &packet) override;

            ENetPeer *_peer;
            uint64_t _player_id;

            Tetris _server_state;
            Tetris _client_state;
    };
}
