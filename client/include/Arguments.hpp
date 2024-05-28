#pragma once

#include <cstdint>
#include <string>

namespace tetriq {
    class Arguments {
        public:
            bool parse(int argc, char *argv[]);

            std::string ip;
            uint16_t port;
    };
}
