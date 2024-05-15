// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/APacket.hpp"
#include "network/NetworkStream.hpp"
#include "network/PacketId.hpp"

#include <cstdint>

namespace tetriq {
    class InitGamePacket : public APacket {
        public:
            InitGamePacket();
            InitGamePacket(uint64_t game_width, uint64_t game_height, const std::vector<uint64_t> &player_ids);

            PacketId getId() const override;
            uint64_t getGameWidth() const;
            uint64_t getGameHeight() const;
            const std::vector<uint64_t> &getPlayerIds() const;

            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;
        private:
            uint64_t _game_width;
            uint64_t _game_height;
            std::vector<uint64_t> _player_ids;
    };
}
