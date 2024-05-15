// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/APacket.hpp"

namespace tetriq {
    void APacket::send(ENetPeer *peer) const
    {
        NetworkOStream stream{sizeof(uint64_t) + getNetworkSize()};

        getId() >> stream;
        *this >> stream;

        ENetPacket *epacket =
            enet_packet_create(stream.getData(), stream.getSize(), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, epacket);
    }
}
