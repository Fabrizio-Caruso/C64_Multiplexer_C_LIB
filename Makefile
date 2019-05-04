# Makefile 
CC65_PATH ?=
SOURCE_PATH ?= ./src
GRAPHICS_PATH ?= ./graphics
ASMFILES=$(SOURCE_PATH)/multi_ca65_split.s $(GRAPHICS_PATH)/graphics.s

BUILD_PATH ?= ./build


MYCCFLAGS=-t c64 -O -Cl
MYDEBUGCCFLAGS=-t c64

MYCFG=--config ./cfg/c64_multiplexer_gfx_at_2000.cfg --asm-define USE_KERNAL=1


ifneq ($(COMSPEC),)
DO_WIN:=1
endif
ifneq ($(ComSpec),)
DO_WIN:=1
endif 

ifeq ($(DO_WIN),1)
EXEEXT = .exe
endif

ifeq ($(DO_WIN),1)
COMPILEDEXT = .exe
else
COMPILEDEXT = .out
endif

MYCC65 ?= cc65$(EXEEXT) $(INCLUDE_OPTS) 
MYCL65 ?= cl65$(EXEEXT) $(INCLUDE_OPTS) 

34_sprites: 
	$(CC65_PATH)$(MYCL65)$ $(MYCCFLAGS) $(MYCFG) --asm-define MAXSPR=34 $(SOURCE_PATH)/34_sprites.c $(ASMFILES) -o $(BUILD_PATH)/34_sprites.prg
	rm $(SOURCE_PATH)/34_sprites.o
	rm $(SOURCE_PATH)/multi_ca65_split.o
	rm $(GRAPHICS_PATH)/*.o

sin_scroller: 
	$(CC65_PATH)$(MYCL65)$ $(MYCCFLAGS) $(MYCFG) --asm-define MAXSPR=16 $(SOURCE_PATH)/sin_scroller.c $(ASMFILES) -o $(BUILD_PATH)/sin_scroller.prg
	rm $(SOURCE_PATH)/sin_scroller.o
	rm $(SOURCE_PATH)/multi_ca65_split.o
	rm $(GRAPHICS_PATH)/*.o    
    
34_sprites_debug:
	$(CC65_PATH)$(MYCC65)$ $(MYDEBUGCCFLAGS) $(SOURCE_PATH)/34_sprites.c -o $(SOURCE_PATH)/34_sprites.s
	$(CC65_PATH)$(MYCL65)$ $(MYDEBUGCCFLAGS) $(MYCFG) --asm-define MAXSPR=34 --asm-define DEBUG=1 $(SOURCE_PATH)/34_sprites.s $(ASMFILES) -o $(BUILD_PATH)/34_sprites_debug.prg
	rm $(SOURCE_PATH)/34_sprites.s
	rm $(SOURCE_PATH)/34_sprites.o
	rm $(SOURCE_PATH)/multi_ca65_split.o
	rm $(GRAPHICS_PATH)/*.o
    
all: 34_sprites sin_scroller

clean:
	rm -rf *.prg
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf ./build/*
	rm -rf main.s
