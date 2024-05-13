// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Tetris.hpp"
#include "network/IPacket.hpp"

namespace tetriq {
    class TickGamePacket : public IPacket {
        public:
            TickGamePacket();
            TickGamePacket(const Tetris &game);

            PacketId getId() const override;
            // This should only be called if the packet was received
            const Tetris &getGame() const;

            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;
        private:
            // TODO : figure out how to send the blocks too
            uint64_t _width;
            uint64_t _height;
            std::vector<Tetromino> _nextPieces;
    };
}
