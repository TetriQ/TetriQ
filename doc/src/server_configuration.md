<!--
SPDX-FileCopyrightText: 2024 The Tetriq authors

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# Server configuration

If you want to run your own server, all you will need is a valid
configuration file! The format of the file and the available
configurations are documented in this section.

When the server is started using `tetriq_server`, it will look for
configuration files in the following locations in order until it finds
a valid one:
1. server.toml
2. etc/server.toml
3. $XDG\_CONFIG\_HOME/tetriq/server.toml
4. /etc/tetriq/server.toml

In the case that `XDG_CONFIG_HOME` is not set, it will default to
`~/.config`.

To make your own configuration, copy the configuration file that
should be distributed along with TetriQ at either `etc/server.toml` or
`/etc/tetriq/server.toml` to one of the directories listed above
(three or four is preferable) and modify it as you see best.

## Configuration fields

Here are the available configuration fields, along with their default
value and description. If any field is missing from the configuration
file, its default value will be used.

 - **max_clients** = 128

The maximum amount of open connections that can be made to the
server. This will directly impact RAM usage by ENet[^enet], no matter how
many players are actually connected.

 - **max_outgoing_bandwidth** = 0

Upstream bandwidth of the host in bytes/second; if 0, ENet[^enet] will assume
unlimited bandwidth.

 - **max_incoming_bandwidth** = 0

Downstream bandwidth of the host in bytes/second; if 0, ENet[^enet] will
assume unlimited bandwidth.

[^enet]: ENet is the network library used by TetriQ to manage
	connections.

- **client_timeout** = 1000

The time in milliseconds for a client to timeout. This is currently
unused and does nothing.

- **listen_address** = "0.0.0.0"

The address on which the server will listen for incoming
connections. Make sure to set this to an address that is reachable by
your peers. Here are a few example values:

| Address      | Usage                                                                                                         |
|--------------|---------------------------------------------------------------------------------------------------------------|
| `127.0.0.1`  | The loopback address. Will only accept connections from your computer                                         |
| `0.0.0.0`    | Special. Will accept connections on any address                                                               |
| `192.0.0.20` | Local ip address. Will accept connections from your local network. Yours will probably have a different value |

When connecting with a client, make sure the address you use matches
the one you chose for your server.

- **listen_port** = 31457

The port on which the server will listen for incoming connections.

- **ticks_per_second** = 60

The frequency at which the server updates its internal state. This is
different than `ticks_per_second` in the `[game]` section which is the
speed for the game itself.

Make sure that this is greater than the game's `ticks_per_second` or
else the game will be running slower than expected. Setting this too
high will increase resource consumption for very few latency gains.

### Game configuration

The game rules can be configured in the `[game]` section of the
server's configuration file. Here are the available configuration
fields, along with their default value and description. If any field
is missing from the configuration file, its default value will be
used.

- **ticks_per_second** = 5

The frequency at which the game is updated, i.e. the number of times
per second blocks fall down.

- **width** = 12

The width of the game area in blocks. This includes the game's
unbreakable border.

- **height** = 22

The height of the game area in blocks. This includes the game's
unbreakable border.

### RCON configuration

The server can be controlled remotely using the RCON protocol.
The RCON is using simple TCP sockets. This means that you can use
`telnet` or `netcat` to connect to the RCON server and send commands to it.

Notes:
- every command must be terminated by a newline character. Beware telnet users.
- for now, the RCON is not secured. It is recommended to use a firewall 
to restrict access to the RCON port.
- the RCON is also not encrypted. It uses plain text.
- there can be only one RCON connection at a time.

The RCON configuration can be found in the `[rcon]` section of the
server's configuration file. Here are the available configuration
fields, along with their default value and description. If any field
is missing from the configuration file, its default value will be
used.

- **enabled** = false

Whether the RCON server is enabled or not.

- **password** = "root"

The password required to authenticate with the RCON server.
It's the first thing you should change when enabling the RCON server.

- **listen_address** = "0.0.0.0"

The address on which the RCON server will listen for incoming connections.

- **listen_port** = 31458

The port on which the RCON server will listen for incoming connections (TCP)


