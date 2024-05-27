// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/APacket.hpp"
#include "network/PacketId.hpp"

#include <cstdint>

namespace tetriq {
    struct TestPacket : public APacket {
        public:
            PacketId getId() const override;
            
            NetworkOStream &operator>>(NetworkOStream &ns) const override;
            NetworkIStream &operator<<(NetworkIStream &ns) override;
            size_t getNetworkSize() const override;
        private:
            uint64_t _magic = 0x737819;
    };
}
