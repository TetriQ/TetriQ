#include "network/PacketHandler.hpp"
#include "Logger.hpp"

namespace tetriq {
    template <class P>
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
        uint64_t id;
        stream >> id;
        switch (static_cast<PacketId>(id)) {
        case PacketId::TEST:
            return handlePacket<TestPacket>(*this, stream);
        default:
            LogLevel::WARNING << "reveived packet with unknown id '" << id << "'" << std::endl;
            return false;
        }
    }

    bool PacketHandler::handle(TestPacket &)
    {
        return false;
    }
}
