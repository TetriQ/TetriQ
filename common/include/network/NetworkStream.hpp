// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <enet/enet.h>

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

            NetworkOStream &operator<<(uint64_t value);
         private:
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

            NetworkIStream &operator>>(uint64_t &value);
        private:
            ENetPacket *const _packet;
            size_t _cursor;
    };
}
