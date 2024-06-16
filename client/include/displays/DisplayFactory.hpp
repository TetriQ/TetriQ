// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <map>
#include <functional>
#include <memory>

#include "IDisplay.hpp"
#include "NcursesDisplay.hpp"
#include "SFMLDisplay.hpp"

namespace tetriq
{
    class DisplayFactory {
    public:
        static std::unique_ptr<IDisplay> createFromName(const std::string &name);

    private:
        using DisplayMap = std::map<std::string, std::function<std::unique_ptr<IDisplay>()>>;
        static const DisplayMap _materials;
    };
}