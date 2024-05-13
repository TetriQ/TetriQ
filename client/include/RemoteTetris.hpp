#pragma once

#include "Tetris.hpp"
#include "network/PacketHandler.hpp"

namespace tetriq {
    /**
     * @brief A tetris game that is controlled by a remote server.
     */
    class RemoteTetris : public PacketHandler {
        public:
            RemoteTetris(size_t width, size_t height);

            /**
             * @returns the game state to be displayed by the client
             */
            const Tetris &getState() const;
            
        private:
            bool handle(TestPacket &packet) override;
            
            Tetris _server_state;
            Tetris _client_state;
    };
}
