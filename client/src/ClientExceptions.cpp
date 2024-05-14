// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <utility>

#include "Client.hpp"

namespace tetriq {
    Client::ClientException::ClientException(std::string message)
        : _message(std::move(message))
    {}

    const char *Client::ClientException::what() const noexcept
    {
        return _message.c_str();
    }

    Client::ClientInitException::ClientInitException()
        : ClientException("Failed to initialize client")
    {}

    Client::ClientConnectionException::ClientConnectionException()
        : ClientException("Failed to connect to server")
    {}
}
