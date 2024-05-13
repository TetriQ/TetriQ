// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/NetworkStream.hpp"
#include <cstddef>
#include <cstdint>
#include <endian.h>

tetriq::NetworkOStream::NetworkOStream(size_t size)
    : _size(size)
    , _buf(new uint8_t[size])
    , _cursor(0)
{}

const uint8_t *tetriq::NetworkOStream::getData() const
{
    return _buf.get();
}

size_t tetriq::NetworkOStream::getSize() const
{
    return _size;
}

tetriq::NetworkOStream &tetriq::NetworkOStream::operator<<(uint64_t value) 
{
    if (_cursor > _size - sizeof(uint64_t))
        throw NetworkStreamOverflowException();
    *(uint64_t *) &_buf[_cursor] = htobe64(value);
    _cursor += sizeof(uint64_t);
    return *this;
}

tetriq::NetworkIStream::NetworkIStream(ENetPacket *packet)
    : _packet(packet)
    , _cursor(0)
{}

tetriq::NetworkIStream::~NetworkIStream()
{
    enet_packet_destroy(_packet);
}

const uint8_t *tetriq::NetworkIStream::getData() const
{
    return _packet->data;
}

size_t tetriq::NetworkIStream::getSize() const
{
    return _packet->dataLength;
}

tetriq::NetworkIStream &tetriq::NetworkIStream::operator>>(uint64_t &value)
{
    if (_cursor > _packet->dataLength - sizeof(uint64_t))
        throw NetworkStreamOverflowException();
    value = be64toh(*(uint64_t *) &_packet->data[_cursor]);
    _cursor += sizeof(uint64_t);
    return *this;
}
