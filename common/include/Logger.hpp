// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <string>
#include <iostream>

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
             * @brief Log a message with a specific log level, if
             * the log level is ERROR or CRITICAL, the message will
             * be logged to the error stream.
             * @param level Log level of the message, see LogLevel enum
             * @param message Message to log
             */
            static void log(LogLevel level, const std::string &message);

            /**
             * @brief Convert a log level to a string
             * @param level Log level to convert
             * @return Log level as a string
             */
            static const char *levelToString(LogLevel level);

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
            static const char *levelToColor(LogLevel level);

            /**
             * @brief Disable logs
             */
            static void setLogVisibility(bool visibility);

        private:
            static bool _visible;
    };

    template<typename T>
    std::ostream &operator<<(const LogLevel &level, const T &msg)
    {
        std::ostream &stream =
            (level == LogLevel::ERROR || level == LogLevel::CRITICAL) ? std::cerr : std::cout;
        return stream << Logger::getTimestamp() << " [" << Logger::levelToColor(level)
                      << Logger::levelToString(level) << "\033[0m] " << msg;
    }
}
