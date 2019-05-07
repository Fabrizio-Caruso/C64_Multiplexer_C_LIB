# Makefile 
CC65_PATH ?=
SOURCE_PATH ?= ./src
GRAPHICS_PATH ?= ./graphics
ASMFILES=$(SOURCE_PATH)/multi_ca65_split.s $(GRAPHICS_PATH)/graphics.s

ASMTEST=$(SOURCE_PATH)/irq_test.s $(GRAPHICS_PATH)/graphics.s

BUILD_PATH ?= ./build


MYCCFLAGS=-t c64 -O -Cl
MYC128CCFLAGS=-t c128 -O -Cl
MYDEBUGCCFLAGS=-t c64

MYCFG=--config ./cfg/c64_multiplexer_gfx_at_2000.cfg --asm-define USE_KERNAL=1
MYC128CFG=--config ./cfg/c128_multiplexer_gfx_at_3000.cfg --asm-define USE_KERNAL=1


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
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) --asm-define MAXSPR=34 $(SOURCE_PATH)/34_sprites.c $(ASMFILES) -o $(BUILD_PATH)/34_sprites.prg
	rm $(SOURCE_PATH)/34_sprites.o
	rm $(SOURCE_PATH)/multi_ca65_split.o
	rm $(GRAPHICS_PATH)/*.o

sin_scroller: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) --asm-define MAXSPR=16 $(SOURCE_PATH)/sin_scroller.c $(ASMFILES) -o $(BUILD_PATH)/sin_scroller.prg
	rm $(SOURCE_PATH)/sin_scroller.o
	rm $(SOURCE_PATH)/multi_ca65_split.o
	rm $(GRAPHICS_PATH)/*.o

sin_scroller_multicolor: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) --asm-define MAXSPR=16 --asm-define MULTICOLOR=1  $(SOURCE_PATH)/sin_scroller_multicolor.c $(ASMFILES) -o $(BUILD_PATH)/sin_scroller_multicolor.prg
	rm $(SOURCE_PATH)/sin_scroller_multicolor.o
	rm $(SOURCE_PATH)/multi_ca65_split.o
	rm $(GRAPHICS_PATH)/*.o 
    
34_sprites_debug:
	$(CC65_PATH)$(MYCC65) $(MYDEBUGCCFLAGS) $(SOURCE_PATH)/34_sprites.c -o $(SOURCE_PATH)/34_sprites.s
	$(CC65_PATH)$(MYCL65) $(MYDEBUGCCFLAGS) $(MYCFG) --asm-define MAXSPR=34 --asm-define DEBUG=1 $(SOURCE_PATH)/34_sprites.s $(ASMFILES) -o $(BUILD_PATH)/34_sprites_debug.prg
	rm $(SOURCE_PATH)/34_sprites.s
	rm $(SOURCE_PATH)/34_sprites.o
	rm $(SOURCE_PATH)/multi_ca65_split.o
	rm $(GRAPHICS_PATH)/*.o
    
hello_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) $(SOURCE_PATH)/hello_world.c $(GRAPHICS_PATH)/graphics.s -o $(BUILD_PATH)/hello_c128.prg
	rm $(SOURCE_PATH)/hello_world.o
	rm $(GRAPHICS_PATH)/*.o

sin_scroller_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) --asm-define MAXSPR=16 $(SOURCE_PATH)/sin_scroller.c $(ASMFILES) -o $(BUILD_PATH)/sin_scroller_c128.prg
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o

irq_test_case:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) $(SOURCE_PATH)/irq_test_case.c $(ASMTEST) -o $(BUILD_PATH)/irq_test_case.prg
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o

all: 34_sprites sin_scroller sin_scroller_multicolor

clean:
	rm -rf *.prg
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf ./build/*
	rm -rf main.s
