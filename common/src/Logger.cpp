// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Logger.hpp"

#include <iostream>

bool tetriq::Logger::_visible = true;

void tetriq::Logger::log(const LogLevel level, const std::string &message)
{
    if (!_visible)
        return;
    level << message << std::endl;
}

const char *tetriq::Logger::levelToString(const LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::CRITICAL:
            return "CRITICAL";
    }
    return "UNKNOWN";
}

std::string tetriq::Logger::getTimestamp()
{
    const time_t now = time(nullptr);
    const tm *ltm = localtime(&now);
    char timestamp[20];
    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", ltm);
    return {timestamp};
}

const char *tetriq::Logger::levelToColor(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return "\033[38;5;244m";
        case LogLevel::INFO:
            return "\033[38;5;39m";
        case LogLevel::WARNING:
            return "\033[38;5;208m";
        case LogLevel::ERROR:
            return "\033[38;5;196m";
        case LogLevel::CRITICAL:
            return "\033[38;5;160m";
    }
    return "\033[38;5;255m";
}

void tetriq::Logger::setLogVisibility(bool visibility)
{
    _visible = visibility;
}

bool tetriq::Logger::isVisible()
{
    return _visible;
}
