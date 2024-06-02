// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/packets/FullGameRequestPacket.hpp"
#include "network/packets/TestPacket.hpp"
#include "network/packets/GameActionPacket.hpp"
#include "network/packets/InitGamePacket.hpp"
#include "network/packets/FullGamePacket.hpp"
#include "network/packets/TickGamePacket.hpp"
#include "network/packets/PowerUpPacket.hpp"

namespace tetriq {
    class PacketHandler {
        public:
            /**
             * @brief Decodes a packet and handles it using the handlers.
             * @param event An enet event of type ENET_EVENT_TYPE_RECEIVE
             */
            static bool decodePacket(
                const ENetEvent &event, const std::vector<PacketHandler *> &handlers);

            virtual bool handle(TestPacket &p);
            virtual bool handle(InitGamePacket &p);
            virtual bool handle(TickGamePacket &p);
            virtual bool handle(FullGamePacket &p);
            virtual bool handle(GameActionPacket &p);
            virtual bool handle(FullGameRequestPacket &p);
            virtual bool handle(PowerUpPacket &p);
    };
}
