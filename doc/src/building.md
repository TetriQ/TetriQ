<!--
SPDX-FileCopyrightText: 2024 The TetriQ authors

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# Building from source

You can download the source code from github:
<https://github.com/TetriQ/TetriQ>

## Dependencies

To build TetriQ, you will need the following dependencies:
 - CMake
 - ENet
 - tomlplusplus
Additionally for the client:
 - SFML
 - ncurses

## Building

TetriQ uses the CMake build system, you can easily compile the code
with the following commands:
```
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build/ -j
```

You will then find the server and client binaries under the names
`tetriq_server` and `tetriq_client` in your current directory.
