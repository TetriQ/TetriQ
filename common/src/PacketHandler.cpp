// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/PacketHandler.hpp"
#include "Logger.hpp"
#include "network/packets/InitGamePacket.hpp"
#include "network/packets/TickGamePacket.hpp"

namespace tetriq {
    template<class P>
    static bool handlePacket(PacketHandler &handler, NetworkIStream &stream)
    {
        P packet;
        packet << stream;
        if (!handler.handle(packet)) {
            LogLevel::WARNING << "unhandled packet of type " << typeid(P).name() << std::endl;
            return false;
        }
        return true;
    }

    bool PacketHandler::decodePacket(const ENetEvent &event)
    {
        NetworkIStream stream{event.packet};
        uint64_t id{0};
        id << stream;
        switch (static_cast<PacketId>(id)) {
            case PacketId::TEST:
                return handlePacket<TestPacket>(*this, stream);
            case PacketId::S_INIT_GAME:
                return handlePacket<InitGamePacket>(*this, stream);
            case PacketId::S_TICK_GAME:
                return handlePacket<TickGamePacket>(*this, stream);
            default:
                LogLevel::WARNING << "reveived packet with unknown id '" << id << "'" << std::endl;
                return false;
        }
    }

    bool PacketHandler::handle(TestPacket &)
    {
        return false;
    }

    bool PacketHandler::handle(InitGamePacket &)
    {
        return false;
    }

    bool PacketHandler::handle(TickGamePacket &)
    {
        return false;
    }
}
