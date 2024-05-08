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

#include "Logger.hpp"

tetriq::Logger::Logger(std::ostream &out, std::ostream &err) :
    _logStream(out), _errStream(err)
{
    if (!_logStream.good() || !_errStream.good())
        throw std::runtime_error("Failed to access log streams");
    std::ifstream ansiFile("tetriq.ansi");
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
        + " [" + levelToString(level) + "] " + message;
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
