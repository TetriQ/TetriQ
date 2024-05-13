// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <enet/enet.h>
#include <vector>

namespace tetriq {
    class NetworkStreamOverflowException : public std::exception {};

    /**
     * @brief Binary stream for a buffer of known size to be sent over the wire.
     * The stream should use the network endianness for all values.
     */
    class NetworkOStream {
        public:
            NetworkOStream(size_t size);

            const uint8_t *getData() const;
            size_t getSize() const;
         private:
            friend NetworkOStream &operator>>(uint64_t value, NetworkOStream &stream);
            friend NetworkOStream &operator>>(uint8_t value, NetworkOStream &stream);
            template<typename T>
            friend NetworkOStream &operator>>(std::vector<T> value, NetworkOStream &stream);

            size_t _size;
            std::unique_ptr<uint8_t[]> _buf;
            size_t _cursor;
    };

    /**
     * @brief Binary stream to read a packet received from the wire.
     * The stream should use the network endianness for all values.
     */
    class NetworkIStream {
        public:
            NetworkIStream(ENetPacket *packet);
            ~NetworkIStream();

            const uint8_t *getData() const;
            size_t getSize() const;
        private:
            friend NetworkIStream &operator<<(uint64_t &value, NetworkIStream &stream);
            friend NetworkIStream &operator<<(uint8_t &value, NetworkIStream &stream);
            template<typename T>
            friend NetworkIStream &operator<<(std::vector<T> &value, NetworkIStream &stream);

            ENetPacket *const _packet;
            size_t _cursor;
    };

    NetworkOStream &operator>>(uint64_t value, NetworkOStream &stream);
    NetworkOStream &operator>>(uint8_t value, NetworkOStream &stream);
    template<typename T>
    NetworkOStream &operator>>(std::vector<T> value, NetworkOStream &stream);

    NetworkIStream &operator<<(uint64_t &value, NetworkIStream &stream);
    NetworkIStream &operator<<(uint8_t &value, NetworkIStream &stream);
    template<typename T>
    NetworkIStream &operator<<(std::vector<T> &value, NetworkIStream &stream);

    template<typename T>
    NetworkOStream &operator>>(std::vector<T> value, NetworkOStream &stream)
    {
        uint64_t len = value.size();
        len >> stream;
        for (const T& v : value) {
            v >> stream;
        }
        return stream;
    }

    template<typename T>
    NetworkIStream &operator<<(std::vector<T> &value, NetworkIStream &stream)
    {
        uint64_t len;
        len << stream;
        value.clear();
        value.reserve(len);
        for (uint64_t i = 0; i < len; i++) {
            value.emplace_back();
            value.back() << stream;
        }
        return stream;
    }
}
