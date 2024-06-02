// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "Tetris.hpp"
#include "network/APacket.hpp"
#include <cstdint>

namespace tetriq {
    class PowerUpPacket : public APacket {
        public:
            PowerUpPacket();
            PowerUpPacket(uint64_t target);

            PacketId getId() const override;
            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;

            uint64_t getTarget() const;

        private:
            uint64_t _target;
    };
}
