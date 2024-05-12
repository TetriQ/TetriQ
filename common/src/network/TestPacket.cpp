// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/TestPacket.hpp"
#include "Logger.hpp"
#include "network/NetworkStream.hpp"
#include "network/PacketId.hpp"
#include <cstdint>

tetriq::PacketId tetriq::TestPacket::getId() const
{
    return PacketId::TEST;
}

tetriq::NetworkStream &tetriq::TestPacket::operator>>(tetriq::NetworkStream &ns) const
{
    ns << _magic;
    return ns;
}

tetriq::NetworkStream &tetriq::TestPacket::operator<<(tetriq::NetworkStream &ns)
{
    uint64_t magic;
    ns >> magic;
    if (magic != _magic)
        Logger::log(LogLevel::WARNING, "received incorrect magic value");
    _magic = magic;
    return ns;
}

size_t tetriq::TestPacket::getSize() const
{
    return sizeof(uint64_t);
}
