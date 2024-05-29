// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/APacket.hpp"
#include <cstdint>

namespace tetriq {
    class TickGamePacket : public APacket {
        public:
            TickGamePacket();
            template<class InputIterator>
            TickGamePacket(uint64_t player_id, InputIterator actionsBegin, InputIterator actionsEnd);

            PacketId getId() const override;

            uint64_t getPlayerId() const;

            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;
        private:
            uint64_t _player_id;
    };
}
