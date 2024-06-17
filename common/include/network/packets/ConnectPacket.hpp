// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/APacket.hpp"

namespace tetriq {
    class ConnectPacket : public APacket {
        public:
            ConnectPacket();
            ConnectPacket(uint64_t player_id, uint64_t game_width, uint64_t game_height);

            PacketId getId() const override;
            uint64_t getGameWidth() const;
            uint64_t getGameHeight() const;

            uint64_t getPlayerId() const;

            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;

        private:
            uint64_t _game_width;
            uint64_t _game_height;
            uint64_t _player_id;
    };
}
