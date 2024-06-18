<!--
SPDX-FileCopyrightText: 2024 The Tetriq authors

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# RCON usage

The Remote Console (RCON) is a feature that allows you to send
commands to a running TetriQ server.

## Enabling RCON

To enable RCON, you need to set the `enabled` field in the `rcon`
section of the configuration file to `true`.

## Connecting to the RCON server

You can connect to the RCON server using `netcat` or a similar
tool. The default port is 31458.

```sh
$ nc localhost 31458
```

## Authenticating

To authenticate, you need to send the password set in the
configuration file as your first command.

## Sending commands

After authenticating, you can send commands to the server. Commands
are sent as a single line of text. Here a list of available commands:

- `help`: Display a list of available commands.
- `quit`: Disconnect from the RCON server.
- `set channel <id>`: This command is a bit special. Our TetriQ server
  uses "channels" to separate different games. You can set the channel
  you want to send commands to by using this command. For example,
  `set channel 0` will set the current selected channel to channel
  `0`.
- `list`: List all channels by displaying the number of players in
each channel and an indicator if the game is running or not.
- `create channel`: Create a new channel. (kind of useless, because
players can't join a specific channel for now).
- `delete channel <id>`: Delete a channel. `id` being the channel ID.
- `get`: not implemented yet.
- `startgame`: Start a game in the current channel.
- `stopgame`: Stop the game in the current channel.
