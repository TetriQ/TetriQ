// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

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
            /**
             * @brief Convert a log level to a color
             * @param level Log level to convert
             * @return Log level as a color
             */
            static std::string levelToColor(LogLevel level);

        private:
            std::ostream &_logStream;
            std::ostream &_errStream;

            /**
             * @brief Whether to use ANSI colors or not
             */
            bool _ansi {true};
    };
}
