// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "NetworkObject.hpp"
#include "network/PacketId.hpp"

#include <enet/enet.h>

namespace tetriq {
    class APacket : public NetworkObject {
        public:
            void send(ENetPeer *peer) const;

            virtual PacketId getId() const = 0;
    };
}
