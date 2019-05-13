# C64_Multiplexer_C_LIB
Some routines to handle a C64 and C128 sprite multiplexer with support for SID tunes

We use a modified and extended version of the Ocean Cadaver's sprite multiplexer from:
https://cadaver.github.io/rants/sprite.html

Most signification modifications of the multiplexer and support for sid tunes have been implemented by FlavioWeb.
C128 fast mode, sprite expand flags, minor C code tuning, linker configuration have been implemented by Fabrizio Caruso.

FEATURES
- Freely moving multiplexed sprites (up to 36), i.e., generic sprite multiplexer 
- Support for C64, C128 in both 128 in 64 mode
- Fast 2mhz mode support for the C128 in both 128 and 64 mode: it runs on average at 1.3mhz (PAL) by enabling the fast mode on the borders
- Optionally multiplexed monochromatic/multicolor bit
- Optionally multiplexed x-expand bit
- Optionally multiplexed y-expand bit (to be used with care because of imperfect sorting)
- Efficient horizontal movement: x-coordinate is handled efficiently by the Assembly code and stored in a single byte (2-pixel movemnt)
- Support for SID tunes 

