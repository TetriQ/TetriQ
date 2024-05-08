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

tetriq::Logger::Logger(const std::string &name)
{
    _logFile.open(name + ".log", std::ios::out | std::ios::app);
    if (!_logFile.is_open())
        throw std::runtime_error("Failed to open log file / create log file");
}

tetriq::Logger::~Logger()
{
    if (_logFile.is_open())
        _logFile.close();
}

void tetriq::Logger::log(const LogLevel level, const std::string &message)
{
    if (!_logFile.is_open())
        return;
    _logFile << getTimestamp()
            << " [" << levelToString(level)
            << "] " << message << std::endl;
    _logFile << std::flush;
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
