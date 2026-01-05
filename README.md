# C64_Multiplexer_C_LIB
_Multiplexer adaptation, sid support and most demo code by Flavio Web Pasqualin (@Flaviowebit)_ \
_Raster split adaptation, sprite expand code, some C128 code, CC65 and C support by Fabrizio Caruso (@Fabrizio-Caruso)_ \
_Initial work on the multiplxer by Antonino Porcino (@nippur72)_ 


We provide a little template with a generic sprite multiplexer and a SID tune player for both the Commodore 64 and the native Commodore 128.

We use a modified and extended version of Cadaver's Ocean sprite multiplexer originally taken from:
https://cadaver.github.io/rants/multi.zip

See more details in:
https://cadaver.github.io/rants/sprite.html

--------------------------------

FEATURES
- Freely moving multiplexed sprites (up to 36), i.e., generic sprite multiplexer 
- Simple C interface provided by some shared variables (see INSTRUCTIONS.txt in the "doc" directory)
- Support for C64, C128 in both 128 and in 64 mode
- Fast 2mhz mode support for the C128 in both 128 and 64 mode: it runs on average at 1.3mhz (PAL) by enabling the fast mode on the borders
- Optionally multiplexed monochromatic/multicolor bit
- Optionally multiplexed x-expand bit
- Optionally multiplexed y-expand bit (to be used with care because of imperfect sorting)
- Efficient horizontal movement: x-coordinate is handled efficiently by the Assembly code and stored in a single byte (2-pixel movement)
- Support for SID tunes 

--------------------------------

BEFANA VS BALLOONS 2026

You can play **Befana vs Balloons 2026** on a browser. 
Click here to play <a href="https://nippur72.github.io/c64-emu/?joy=3&load=http://github.com/Fabrizio-Caruso/C64_Multiplexer_C_LIB/releases/download/befana2026/befana.prg"><b>Befana vs Balloons 2026</b>.

The full link is:

https://nippur72.github.io/c64-emu/?joy=3&load=http://github.com/Fabrizio-Caruso/C64_Multiplexer_C_LIB/releases/download/befana2026/befana.prg
