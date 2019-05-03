# Makefile 
CC65_PATH ?=
CCFLAGS=-t c64 --cpu 6502X --add-source
SOURCE_PATH ?= ./src
ASMFILES=$(SOURCE_PATH)/multi_ca65_split.s ./graphics/graphics.s
BUILD_PATH ?= ./build


MYCCFLAGS=-t c64 -O -Cl
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

34sprites: 
	$(CC65_PATH)$(MYCL65)$ $(MYCCFLAGS) $(MYCFG) $(SOURCE_PATH)/34sprites.c $(ASMFILES) -o $(BUILD_PATH)/34sprites.prg
	rm $(SOURCE_PATH)/34sprites.o
	rm $(SOURCE_PATH)/multi_ca65_split.o

34sprites_debug:
	$(CC65_PATH)$(MYCC65)$ $(CCFLAGS) $(SOURCE_PATH)/34sprites.c -o $(SOURCE_PATH)/34sprites.s
	$(CC65_PATH)$(MYCL65)$ $(CCFLAGS) $(MYCFG) --asm-define DEBUG=1 $(SOURCE_PATH)/34sprites.s $(ASMFILES) -o $(BUILD_PATH)/34sprites_debug.prg
	rm $(SOURCE_PATH)/34sprites.s
	rm $(SOURCE_PATH)/34sprites.o
	rm $(SOURCE_PATH)/multi_ca65_split.o
    
all: 34sprites

clean:
	rm -rf *.prg
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf ./build/*
	rm -rf main.s