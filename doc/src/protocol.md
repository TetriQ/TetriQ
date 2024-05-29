<!--
SPDX-FileCopyrightText: 2024 The TetriQ authors

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# Protocol

The TetriQ network protocol uses a client-server architecture. It is
designed to prevent obvious cheating while still being playable on
high latency and low bandwidths connections. For this, the server
sends frequent incremental updates to the client, and the client keeps
an internal prediction of the game state.

## Starting games

Initially, clients connected to the server have no game running. To
start the game, the server sends to the client an `InitGamePacket`
containing the information needed to prepare the game board, followed
by a `FullGamePacket` to synchronise its contents.

## Game loop

The game loop is led by the server. At the tick speed of the game, it
will collect all actions received from players and apply them to their
boards. Once done, the server sends a `TickGamePacket` event to
players, containing the number of actions applied so that the clients
can update their internal state.

From the clients' side, the player actions are sent to the server as
they happen in `GameActionPacket`s, and the internal state is updated
without waiting for the server's response. Once the `TickGamePacket`
is received, the client rolls back its internal state to the server's
and reapplies any unhandled actions.

In case of a synchronisation issue, the client can send a
`FullGameRequestPacket` which will cause the server to send a new
`FullGamePacket` containing the whole board.

For showing the other player's boards, the server broadcasts a
`FullGamePacket` every time the board is meaningfully changed (for
example when a block is placed).
