// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/NetworkStream.hpp"
#include <endian.h>

tetriq::NetworkStream::NetworkStream(size_t size)
    : _size(size)
    , _buf(new char[size])
    , _cursor(0)
{}

const char *tetriq::NetworkStream::getData() const
{
    return _buf.get();
}

size_t tetriq::NetworkStream::getSize() const
{
    return _size;
}

tetriq::NetworkStream &tetriq::NetworkStream::operator<<(uint64_t value) 
{
    if (_cursor >= _size - 1)
        throw NetworkStreamOverflowException();
    _buf[_cursor++] = htobe64(value);
    return *this;
}

tetriq::NetworkStream &tetriq::NetworkStream::operator>>(uint64_t &value)
{
    if (_cursor >= _size - 1)
        throw NetworkStreamOverflowException();
    value = be64toh(_buf[_cursor++]);
    return *this;
}
