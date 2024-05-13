#include "RemoteTetris.hpp"
#include "Logger.hpp"
#include "network/TestPacket.hpp"
#include <cstddef>

namespace tetriq {
    RemoteTetris::RemoteTetris(size_t width, size_t height)
        : _server_state(width, height)
        , _client_state(width, height)
    {}

    const Tetris &RemoteTetris::getState() const
    {
        return _client_state;
    }

    bool RemoteTetris::handle(TestPacket &)
    {
        LogLevel::DEBUG << "handled test packet" << std::endl;
        return true;
    }
}
