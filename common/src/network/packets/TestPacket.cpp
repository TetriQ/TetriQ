// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/TestPacket.hpp"
#include "Logger.hpp"
#include "network/NetworkStream.hpp"
#include "network/PacketId.hpp"
#include <cstdint>

tetriq::TestPacket::TestPacket() = default;

tetriq::PacketId tetriq::TestPacket::getId() const
{
    return PacketId::TEST;
}

tetriq::NetworkOStream &tetriq::TestPacket::operator>>(tetriq::NetworkOStream &ns) const
{
    _magic >> ns;
    return ns;
}

tetriq::NetworkIStream &tetriq::TestPacket::operator<<(tetriq::NetworkIStream &ns)
{
    uint64_t magic;
    magic << ns;
    if (magic != _magic)
        Logger::log(LogLevel::WARNING, "received incorrect magic value");
    _magic = magic;
    LogLevel::DEBUG << "magic: " << _magic << std::endl;
    return ns;
}

size_t tetriq::TestPacket::getNetworkSize() const
{
    return sizeof(uint64_t);
}
