// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "rcon/CommandHandler.hpp"

#include <rcon/Rcon.hpp>

namespace tetriq::command {
    void CommandHandler::registerCommand(const std::string &commandName,
        std::unique_ptr<std::function<void(
            const std::vector<std::string> &args, std::queue<std::string> &res_queue)>>
            command)
    {
        commands[commandName] = std::move(command);
    }

    void CommandHandler::executeCommand(const std::string &commandName,
        const std::vector<std::string> &args, std::queue<std::string> &res_queue)
    {
        if (commands.contains(commandName)) {
            const auto &command = commands[commandName];
            command->operator()(args, res_queue);
        } else {
            // LogLevel::ERROR << "Command not found: " << commandName << std::endl;
            RCONLOG(ERROR) << "Command not found: " << commandName << std::endl;
            res_queue.emplace("Command not found: " + commandName + "\n");
        }
    }

    std::map<std::string, std::unique_ptr<std::function<void(const std::vector<std::string> &args,
                              std::queue<std::string> &res_queue)>>> &
        CommandHandler::getCommands()
    {
        return commands;
    }
}
