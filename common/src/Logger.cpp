// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "Logger.hpp"

tetriq::Logger::Logger(std::ostream &out, std::ostream &err)
    : _logStream(out), _errStream(err)
{
    if (!_logStream.good() || !_errStream.good())
        throw std::runtime_error("Failed to access log streams");
    std::ifstream ansiFile("etc/tetriq.ansi");
    if (ansiFile.is_open()) {
        std::string line;
        while (std::getline(ansiFile, line))
            _logStream << line << std::endl;
        ansiFile.close();
    } else {
        _logStream << "Failed to open tetriq.ansi :(" << std::endl;
    }
}

tetriq::Logger::~Logger() = default;

void tetriq::Logger::log(const LogLevel level, const std::string &message) const
{
    if (!_logStream.good() || !_errStream.good())
        return;
    const std::string logMessage = getTimestamp()
        + " [" + (_ansi ? levelToColor(level) : "")
        + levelToString(level) + "\033[0m] "
        + message;
    if (level == LogLevel::ERROR || level == LogLevel::CRITICAL) {
        _errStream << logMessage << std::endl;
        _errStream.flush();
    } else {
        _logStream << logMessage << std::endl;
        _logStream.flush();
    }
}

std::string tetriq::Logger::levelToString(const LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return {"DEBUG"};
        case LogLevel::INFO:
            return {"INFO"};
        case LogLevel::WARNING:
            return {"WARNING"};
        case LogLevel::ERROR:
            return {"ERROR"};
        case LogLevel::CRITICAL:
            return {"CRITICAL"};
    }
    return {"UNKNOWN"};
}

std::string tetriq::Logger::getTimestamp()
{
    const time_t now = time(nullptr);
    const tm *ltm = localtime(&now);
    char timestamp[20];
    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", ltm);
    return {timestamp};
}

std::string tetriq::Logger::levelToColor(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return {"\033[38;5;244m"};
        case LogLevel::INFO:
            return {"\033[38;5;39m"};
        case LogLevel::WARNING:
            return {"\033[38;5;208m"};
        case LogLevel::ERROR:
            return {"\033[38;5;196m"};
        case LogLevel::CRITICAL:
            return {"\033[38;5;160m"};
    }
    return {"\033[38;5;255m"};
}
