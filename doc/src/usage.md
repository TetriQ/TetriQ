<!--
SPDX-FileCopyrightText: 2024 The TetriQ authors

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# Usage

### Launch client

```
./tetriq_client <ip> <port> <displayMode>
```

**ip**: IP adress of the server (IPv4)

**port**: Port of the server

**displayMode**: Choose between `sfml` or `ncurses`

### Ncurses Keybinds

**Left Arrow**: Move left

**Right Arrow**: Move right

**Down Arrow**: Move down

**Up Arrow**: Rotate

**Space**: Drop

**Q**: Send powerup

**S**: Target player

**0-9**: Target player id (need "select target mode": `S`)

**Z**: Target next player

**X**: Target previous player

Note that when targeting players with `S`, on press you have a 2 seconds delay to
enter a player ID (1 - 999) refreshed every time you add a digit.
Press any non-numerical key or wait the 2 seconds to save the target ID.
