// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/NetworkStream.hpp"
#include <enet/enet.h>

namespace tetriq {
    class NetworkObject {
        public:
            virtual ~NetworkObject() = default;

            /**
             * @brief Writes an object to a stream.
             */
            virtual NetworkOStream &operator>>(NetworkOStream &os) const = 0;

            /**
             * @brief Reads an object from a stream.
             */
            virtual NetworkIStream &operator<<(NetworkIStream &os) = 0;

            /**
             * @returns the size of the object as it will be on the network.
             */
            virtual size_t getSize() const = 0;
    };
}
