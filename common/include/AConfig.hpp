// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <exception>
#include <toml++/toml.hpp>

namespace tetriq {
    class AConfig {
    public:
        AConfig(const std::string &config_name);

        class ConfigNotFoundException : std::exception {};
    protected:
        toml::table _table;
    private:
        bool tryParse(const std::string &file_name);
    };
}
