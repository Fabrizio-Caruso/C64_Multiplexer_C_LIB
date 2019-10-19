# C64_Multiplexer_C_LIB
Multiplexer adaptation, sid support and demos by Flavio Web Pasqualin @Flaviowebit
Raster split adaptation, CC65 and C support by Fabrizio Caruso @Fabrizio-Caruso
Initial work on the multiplxer by Antonino Porcino @nippur72

We provide a little template with a generic sprite multiplexer and a SID tune player for both the Commodore 64 and the native Commodore 128.

We use a modified and extended version of Cadaver's Ocean sprite multiplexer originally taken from:
https://cadaver.github.io/rants/sprite.html

Most signification modifications of the multiplexer and support for SID tunes have been implemented by FlavioWeb.
C128 fast mode, sprite expand flags, minor C code tuning, linker configuration have been implemented by Fabrizio Caruso.

FEATURES
- Freely moving multiplexed sprites (up to 36), i.e., generic sprite multiplexer 
- Simple C interface provided by some shared variables (see INSTRUCTIONS.txt in the "doc" directory)
- Support for C64, C128 in both 128 in 64 mode
- Fast 2mhz mode support for the C128 in both 128 and 64 mode: it runs on average at 1.3mhz (PAL) by enabling the fast mode on the borders
- Optionally multiplexed monochromatic/multicolor bit
- Optionally multiplexed x-expand bit
- Optionally multiplexed y-expand bit (to be used with care because of imperfect sorting)
- Efficient horizontal movement: x-coordinate is handled efficiently by the Assembly code and stored in a single byte (2-pixel movement)
- Support for SID tunes 

