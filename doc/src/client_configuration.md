<!--
SPDX-FileCopyrightText: 2024 The Tetriq authors

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# Client configuration

The client can also be configured, when it is started, it will look
for a configuration file in the following locations in order until it
finds a valid one:
1. client.toml
2. etc/client.toml
3. $XDG\_CONFIG\_HOME/tetriq/client.toml
4. /etc/tetriq/client.toml

In the case that `XDG_CONFIG_HOME` is not set, it will default to
`~/.config`.

To make your own configuration, copy the configuration file that
should be distributed along with TetriQ at either `etc/client.toml` or
`/etc/tetriq/client.toml` to one of the directories listed above
(three or four is preferable) and modify it as you see best.

## Configuration fields

Here are the available configuration fields, along with their default
value and description. If any field is missing from the configuration
file, its default value will be used.

 - **max_outgoing_bandwidth** = 0

Upstream bandwidth of the host in bytes/second; if 0, ENet[^enet] will assume
unlimited bandwidth.

 - **max_incoming_bandwidth** = 0

Downstream bandwidth of the host in bytes/second; if 0, ENet[^enet] will
assume unlimited bandwidth.

- **server_timeout** = 1000

The maximum time in milliseconds to wait for the server's answer
during connection.

[^enet]: ENet is the network library used by TetriQ to manage
	connections.
