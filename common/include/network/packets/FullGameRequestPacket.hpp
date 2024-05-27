// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/APacket.hpp"
namespace tetriq {
    class FullGameRequestPacket : public APacket {
        public:
            PacketId getId() const override;
            virtual NetworkOStream &operator>>(NetworkOStream &os) const override;
            virtual NetworkIStream &operator<<(NetworkIStream &os) override;
            virtual size_t getNetworkSize() const override;
    };
}
