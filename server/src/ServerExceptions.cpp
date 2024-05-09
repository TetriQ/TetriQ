// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Server.hpp"

namespace tetriq {
    Server::ServerException::ServerException(std::string message):
        _message(std::move(message))
    {}

    const char *Server::ServerException::what() const noexcept
    {
        return _message.c_str();
    }

    Server::ServerInitException::ServerInitException(): ServerException(
        "Failed to initialize server")
    {}
}
