// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/TestPacket.hpp"
#include "network/packets/GameActionPacket.hpp"
#include "network/packets/InitGamePacket.hpp"
#include "network/packets/TickGamePacket.hpp"

namespace tetriq {
    class PacketHandler {
        public:
            /**
             * @brief Decodes a packet and handles it.
             * @param event An enet event of type ENET_EVENT_TYPE_RECEIVE
             */
            bool decodePacket(const ENetEvent &event);

            virtual bool handle(TestPacket &p);
            virtual bool handle(InitGamePacket &p);
            virtual bool handle(TickGamePacket &p);
            virtual bool handle(GameActionPacket &p);
    };
}
