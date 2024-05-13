#pragma once

#include "network/TestPacket.hpp"

namespace tetriq {
    class PacketHandler {
        public:
            /**
             * @brief Decodes a packet and handles it.
             * @param event An enet event of type ENET_EVENT_TYPE_RECEIVE
             */
            bool decodePacket(const ENetEvent &event);

            virtual bool handle(TestPacket &p);
    };
}
