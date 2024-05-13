<!--
SPDX-FileCopyrightText: 2024 The TetriQ authors

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# Protocol

This is a draft of what the network protocol might look like.

## Starting games

<!-- TODO: matchmaking -->

Initially, clients connected to the server have no game running. The
game is only started once it receives a `InitGamePacket` from the
server, containing all required information for synchronisation between
the client and the server (board size, next blocks etc...).

## Game loop

The game loop is led by the server. At the tick speed of the game, it
will collect all actions received from players and apply them to their
boards. Once done, the server sends a `TickGamePacket` event to
players, containing every modification applied so that the clients can
update their internal state.

From the clients' side, the player actions are sent to the server as
they happen in `GameActionPacket`s, and the internal state is updated
without waiting for the server's response. Once the `TickGamePacket`
is received, the internal state has to be matched against what the
server sent to make sure no desynchronisation happened, and to roll
back the client in case it happened.
