// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/IPacket.hpp"
#include "network/NetworkStream.hpp"
#include "network/PacketId.hpp"

#include <cstdint>

namespace tetriq {
    class InitGamePacket : public IPacket {
        public:
            InitGamePacket();
            InitGamePacket(uint64_t game_width, uint64_t game_height);

            PacketId getId() const override;

            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;
        private:
            uint64_t _game_width;
            uint64_t _game_height;
    };
}
