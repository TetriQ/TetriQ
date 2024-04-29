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

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <iostream>

namespace tetriq
{
    class NetworkObject {
        public:
            virtual ~NetworkObject() = default;
            // toString return a string representation of the object
            virtual std::string toString() = 0;
            // fromString set the object from a string representation
            virtual void fromString(const std::string &string) = 0;
    };
}

#endif //NETWORK_HPP
