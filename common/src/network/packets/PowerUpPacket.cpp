// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/packets/PowerUpPacket.hpp"

tetriq::PowerUpPacket::PowerUpPacket() = default;

tetriq::PowerUpPacket::PowerUpPacket(uint64_t target)
    : _target(target)
{}

tetriq::PacketId tetriq::PowerUpPacket::getId() const
{
    return PacketId::C_POWER_UP;
}

tetriq::NetworkOStream &tetriq::PowerUpPacket::operator>>(NetworkOStream &ns) const
{
    return _target >> ns;
}

tetriq::NetworkIStream &tetriq::PowerUpPacket::operator<<(NetworkIStream &ns)
{
    return _target << ns;
}

size_t tetriq::PowerUpPacket::getNetworkSize() const
{
    return sizeof(uint64_t);
}

uint64_t tetriq::PowerUpPacket::getTarget() const
{
    return _target;
}
