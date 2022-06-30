Amiga 500 PS/2 keyboard controller
==================================

About
-----
This project is a PS/2 keyboard controller for the Amiga 500. It makes it
possible to connect and use an external PS/2 keyboard with the Amiga.

It's based on an AVR atmega168 microcontroller, which reads the PS/2 keyboard
and translates scancodes to the keyboard signals expected by the Amiga.

See pictures of the PS/2 controller in my Amiga: https://imgur.com/a/rhXS8

It has been reported that this works with the A600 too, by soldering directly
on the keyboard clock and data lines. I guess it might work with the A1200 as
well, so if anyone tries it please let me know.

Directory structure:

  * `hw` - hardware: kicad files and pdf schematics.
  * `fw` - firmware for the AVR microcontroller.

Project website: http://nuclear.mutantstargoat.com/hw/amiga/a500kbd

License
-------
Copyright (C) 2017 John Tsiombikas <nuclear@member.fsf.org>

You are free to use, reproduce, modify, redistribute any parts of this project,
provided you make any derivative work you release, freely available under the
same terms.

More specifically the hardware design is released under the Creative Commons
Attribution Share-Alike license (see `LICENSE.hw` for details), and the
software/firmware is released under the GNU General Public License v3, or later
(see `LICENSE.fw` for details).

Instructions
------------
Just construct the hardware, burn the firmware, drill appropriate holes, and
connect everything together. The board is designed to sit on top of the keyboard
connector pins on the Amiga motherboard, and provides a new set of pins for the
internal keyboard to be connected on the board. Two cables need to be connected
from the board to some hole on the Amiga case: one goes to the PS/2 connector,
and the other to the DPDT switch which selects between internal and external
keyboard (see: https://i.imgur.com/ovFDP2b.jpg).

The default key mapping (can only be changed by modifying the source code at the
moment), is as follows:

  * Caps lock key is mapped to control.
  * Left control key is mapped to left amiga key.
  * Right control key is mapped to right amiga key.
  * Scroll lock is mapped to caps lock.
  * F12 is mapped to "Help".

Also the scroll lock light on the external keyboard, mirrors the drive activity
LED on the amiga.
