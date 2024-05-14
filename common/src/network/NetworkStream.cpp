// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/NetworkStream.hpp"
#include "Logger.hpp"
#include <cstddef>
#include <cstdint>
#include <endian.h>

namespace tetriq {

    NetworkOStream::NetworkOStream(size_t size)
        : _size(size)
        , _buf(new uint8_t[size])
        , _cursor(0)
    {}

    const uint8_t *NetworkOStream::getData() const
    {
        if (_size != _cursor)
            LogLevel::WARNING
                << "NetworkOStream was not completely filled before getting its data (" << _cursor
                << "/" << _size << " bytes)" << std::endl;
        return _buf.get();
    }

    size_t NetworkOStream::getSize() const
    {
        return _size;
    }

    NetworkOStream &operator>>(uint64_t value, NetworkOStream &stream)
    {
        if (stream._cursor > stream._size - sizeof(uint64_t))
            throw NetworkStreamOverflowException();
        *(uint64_t *) &stream._buf[stream._cursor] = htobe64(value);
        stream._cursor += sizeof(uint64_t);
        return stream;
    }

    NetworkOStream &operator>>(uint8_t value, NetworkOStream &stream)
    {
        if (stream._cursor > stream._size - sizeof(uint8_t))
            throw NetworkStreamOverflowException();
        *(uint8_t *) &stream._buf[stream._cursor] = htobe64(value);
        stream._cursor += sizeof(uint8_t);
        return stream;
    }

    NetworkIStream::NetworkIStream(ENetPacket *packet)
        : _packet(packet)
        , _cursor(0)
    {}

    NetworkIStream::~NetworkIStream()
    {
        enet_packet_destroy(_packet);
    }

    const uint8_t *NetworkIStream::getData() const
    {
        return _packet->data;
    }

    size_t NetworkIStream::getSize() const
    {
        return _packet->dataLength;
    }

    NetworkIStream &operator<<(uint64_t &value, NetworkIStream &stream)
    {
        if (stream._cursor > stream._packet->dataLength - sizeof(uint64_t))
            throw NetworkStreamOverflowException();
        value = be64toh(*(uint64_t *) &stream._packet->data[stream._cursor]);
        stream._cursor += sizeof(uint64_t);
        return stream;
    }

    NetworkIStream &operator<<(uint8_t &value, NetworkIStream &stream)
    {
        if (stream._cursor > stream._packet->dataLength - sizeof(uint8_t))
            throw NetworkStreamOverflowException();
        value = be64toh(*(uint8_t *) &stream._packet->data[stream._cursor]);
        stream._cursor += sizeof(uint8_t);
        return stream;
    }

}
