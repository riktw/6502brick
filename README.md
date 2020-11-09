# 6502brick
A 6502 dev board usable with multiple popular projects

This board features a 65C02 CPU, 32K RAM, 32K ROM (16K mapped by default), a 65C22 VIA and a MC6850 ACIA.
A 40 pin header is available with address, data and control signals to extend the board more, or to connect a logic analyzer.
Like Ben Eater's amazing video's, a button is available for single stepping.

The memory map and interrupts are controlled via GALs for flexibility

More information can be read here:  https://justanotherelectronicsblog.com/wp-admin/post.php?post=802

# Memory map:

0x0000 - 0x7FFF		RAM
0x8000 - 0x9FFF		External peripherals
0xA000 - 0xAFFF		MC6850 ACIA for UART
0xB000 - 0xBFFF		65C22 VIA for GPIO + Timers
0xC000 - 0xFFFF		ROM
