// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>

namespace tetriq {
    /**
     * @brief Binary stream for a buffer of known size to be sent over the wire.
     * The stream should use the network endianness for all values.
     */
    class NetworkStream {
        public:
            NetworkStream(size_t size);

            const char *getData() const;
            size_t getSize() const;

            NetworkStream &operator<<(uint64_t value);

            NetworkStream &operator>>(uint64_t &value);
 
            class NetworkStreamOverflowException : public std::exception {};
        private:
            size_t _size;
            std::unique_ptr<char[]> _buf;
            size_t _cursor;
    };
}
