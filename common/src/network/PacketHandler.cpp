// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/PacketHandler.hpp"
#include "Logger.hpp"
#include "network/packets/FullGameRequestPacket.hpp"
#include "network/packets/GameActionPacket.hpp"
#include "network/packets/InitGamePacket.hpp"
#include "network/packets/FullGamePacket.hpp"
#include "network/packets/TickGamePacket.hpp"

namespace tetriq {
    template<class P>
    static bool handlePacket(const std::vector<PacketHandler *> &handlers, NetworkIStream &stream)
    {
        P packet;
        packet << stream;
        for (PacketHandler *handler : handlers) {
            if (handler->handle(packet))
                return true;
        }
        LogLevel::WARNING << "unhandled packet of type " << typeid(P).name() << std::endl;
        return false;
    }

    bool PacketHandler::decodePacket(
        const ENetEvent &event, const std::vector<PacketHandler *> &handlers)
    {
        NetworkIStream stream{event.packet};
        uint64_t id{0};
        id << stream;
        switch (static_cast<PacketId>(id)) {
            case PacketId::TEST:
                return handlePacket<TestPacket>(handlers, stream);
            case PacketId::S_INIT_GAME:
                return handlePacket<InitGamePacket>(handlers, stream);
            case PacketId::S_TICK_GAME:
                return handlePacket<TickGamePacket>(handlers, stream);
            case PacketId::S_FULL_GAME:
                return handlePacket<FullGamePacket>(handlers, stream);
            case PacketId::C_GAME_ACTION:
                return handlePacket<GameActionPacket>(handlers, stream);
            case PacketId::C_FULL_GAME_REQUEST:
                return handlePacket<FullGameRequestPacket>(handlers, stream);
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

    bool PacketHandler::handle(FullGamePacket &)
    {
        return false;
    }

    bool PacketHandler::handle(GameActionPacket &)
    {
        return false;
    }

    bool PacketHandler::handle(FullGameRequestPacket &)
    {
        return false;
    }
}
