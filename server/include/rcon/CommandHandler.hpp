// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <map>
#include <string>
#include "Logger.hpp"
#include <memory>
#include <functional>
#include <queue>

namespace tetriq::command {
    class CommandHandler {
        private:
            std::map<std::string,
                std::unique_ptr<std::function<void(
                    const std::vector<std::string> &args, std::queue<std::string> &res_queue)>>>
                commands;

        public:
            void registerCommand(const std::string &commandName,
                std::unique_ptr<std::function<void(
                    const std::vector<std::string> &args, std::queue<std::string> &res_queue)>>
                    command);

            void executeCommand(const std::string &commandName,
                const std::vector<std::string> &args, std::queue<std::string> &res_queue);

            std::map<std::string,
                std::unique_ptr<std::function<void(
                    const std::vector<std::string> &args, std::queue<std::string> &res_queue)>>> &
                getCommands();
    };
}
