midikeys - a simple midi keyboard for DOS
=========================================

Midikeys is a very simple midi keyboard for DOS PCs with an MPU-401 or
compatible (UART mode) MIDI interface. It allows you to play music using the
two bottom rows of the keyboard, with the bottom row (Z to /) being the white
piano keys, while the next row (A to ') the black keys.

The MPU-401 base I/O port can be specified with the `-p` command-line option.
The default is to heed the `P:` part of the `BLASTER` environment variable, if
it is set, or otherwise fallback to using 330h.

Latest release (1.0):
  - http://nuclear.mutantstargoat.com/sw/midikeys/releases/midikeys-1.0.tar.gz
  - http://nuclear.mutantstargoat.com/sw/midikeys/releases/midkey10.zip

Both release archives are identical, containing source code and DOS executable.


License
-------
Copyright (C) 2022-2024 John Tsiombikas <nuclear@mutantstargoat.com>

This program is free software. Feel free to use, modify, and/or redistribute it
under the terms of the GNU General Public License version 3, or at your option
any later version published by the Free Software Foundation. See COPYING for
details.

Controls
--------
  - Bottom two rows of keys (Z to /) and (A to '): play notes.
  - Number keys (1-8): change MIDI channel.
  - `(` and `)`: change instrument.
  - `[` and `]`: shift octave down/up.
  - `+` and `-`: change velocity.
  - Escape: exit the program.

Command line arguments
----------------------
  - `-p <port>`: set MIDI base I/O port.
  - `-c <channel>`: select initial MIDI channel.
  - `-i <instrument>`: select initial MIDI instrument.
  - `-o <octave>`: select initial octave (1 - 7).

Build
-----
To build midikeys you need the Watcom or OpenWatcom C compiler. There are no
dependencies, just type `wmake` to build.

Running in dosbox
-----------------
To run in dosbox or dosbox-x, if it doesn't work automatically, you might need
to configure which system MIDI device dosbos should use. For me on GNU/Linux
with timidity configured as an ALSA MIDI device, I had to create this simple
`dosbox.conf` file in the current directory:

    [midi]
    mididevice=alsa
    midiconfig=128:0
