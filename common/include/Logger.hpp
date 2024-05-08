/*
* This file is part of TetriQ.
* Copyright © 2024 Sébastien Lucas <sebastien.lucas@epitech.eu>
*
* TetriQ is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* TetriQ is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TetriQ. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace tetriq {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    class Logger {
        public:
            /**
             * @brief Logger constructor
             * @param out Output stream for normal logs
             * @param err Output stream for error logs
             * @exception std::runtime_error if the log file could not be opened
             * @todo change exception type to a custom exception
             */
            explicit Logger(std::ostream &out = std::cout,
                std::ostream &err = std::cerr);
            /**
             * @brief Logger destructor, does nothing
             */
            ~Logger();
            /**
             * @brief Log a message with a specific log level, if
             * the log level is ERROR or CRITICAL, the message will
             * be logged to the error stream.
             * @param level Log level of the message, see LogLevel enum
             * @param message Message to log
             */
            void log(LogLevel level, const std::string &message) const;
            /**
             * @brief Convert a log level to a string
             * @param level Log level to convert
             * @return Log level as a string
             */
            static std::string levelToString(LogLevel level);
            /**
             * @brief Get the current timestamp
             * @return Current timestamp as a string
             */
            static std::string getTimestamp();

        private:
            std::ostream &_logStream;
            std::ostream &_errStream;
    };
}
