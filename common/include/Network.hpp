/*
** EPITECH PROJECT, 2024
** tetriq
** File description:
** Network.hpp
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
