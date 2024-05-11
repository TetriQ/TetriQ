// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "AConfig.hpp"
#include "Logger.hpp"

#include <cstdlib>
#include <string>
#include <unistd.h>

static std::string xdg_config_home()
{
    const char *home = std::getenv("XDG_CONFIG_HOME");
    if (home == NULL)
        home = "~/.config";
    return home;
}

tetriq::AConfig::AConfig(const std::string &config_name)
{
    if (tryParse(config_name))
        return;
    if (tryParse("etc/" + config_name))
        return;
    if (tryParse(xdg_config_home() + "/tetriq/" + config_name))
        return;
    if (tryParse("/etc/tetriq/" + config_name))
        return;
    throw ConfigNotFoundException();
}

bool tetriq::AConfig::tryParse(const std::string &file_name)
{
    if (access(file_name.data(), R_OK) != 0) {
        LogLevel::DEBUG << "looking for configuration " << file_name << std::endl;
        return false;
    }
    try {
        _table = toml::parse_file(file_name);
    } catch (const toml::parse_error &err) {
        LogLevel::ERROR << "failed to load config file " << file_name << std::endl;
        Logger::log(LogLevel::ERROR, err.what());
        return false;
    }
    Logger::log(LogLevel::INFO, "using configuration " + file_name);
    return true;
}
