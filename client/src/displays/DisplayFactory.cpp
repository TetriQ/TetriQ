// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "DisplayFactory.hpp"

const tetriq::DisplayFactory::DisplayMap tetriq::DisplayFactory::_materials =
{
    { "sfml", []() { return std::make_unique<SFMLDisplay>(); }},
    { "ncurses", []() { return std::make_unique<NcursesDisplay>(); }},
};

std::unique_ptr<tetriq::IDisplay> tetriq::DisplayFactory::createFromName(const std::string &name)
{
    try {
        return _materials.at(name)();
    } catch (const std::out_of_range &) {
        return nullptr;
    };
}
