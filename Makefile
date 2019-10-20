# Makefile 
CC65_PATH ?=
SOURCE_PATH ?= ./src
DEMOS_PATH ?= ./demos
GRAPHICS_PATH ?= ./graphics
SID_PATH ?= ./sid
ASMFILES=$(SOURCE_PATH)/generic_multiplexer.s $(GRAPHICS_PATH)/graphics.s

ASMTEST=$(SOURCE_PATH)/irq_test.s $(GRAPHICS_PATH)/graphics.s

BUILD_PATH ?= ./build


MYCCFLAGS=-t c64 -O -Cl
MYC128CCFLAGS=-t c128 -O -Cl
MYDEBUGCCFLAGS=-t c64
MULTICFG=--asm-define MULTICOLOR=1 -DMULTI_COLOR
EXPANDXCFG=--asm-define EXPANDX=1 -DEXPAND_X
EXPANDYCFG=--asm-define EXPANDY=1 -DEXPAND_Y


MYCFG=--config ./cfg/c64_multiplexer_gfx_at_2000.cfg --asm-define USE_KERNAL=1
MYC128CFG=--config ./cfg/c128_multiplexer_gfx_at_3000.cfg --asm-define USE_KERNAL=1
MYSIDCFG=--config ./cfg/c64_multiplexer_sid_at_1000_gfx_at_2000.cfg --asm-define USE_KERNAL=1
MYSIDC128CFG=--config ./cfg/c128_multiplexer_sid_at_2400_gfx_at_3000.cfg --asm-define USE_KERNAL=1


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


some_sprites: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define MAXSPR=16 -D_NUMBER_OF_SPRITES_=16 -D_SPRITE_SEPARATION_=24 \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/many_sprites.c $(ASMFILES) \
	-o $(BUILD_PATH)/some_sprites.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o

many_sprites: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define MAXSPR=20 -D_NUMBER_OF_SPRITES_=20 -D_SPRITE_SEPARATION_=25 \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/many_sprites.c $(ASMFILES) \
	-o $(BUILD_PATH)/many_sprites.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o

sin_scroller:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define FAST_MODE=1 \
	--asm-define MAXSPR=16 \
	$(DEMOS_PATH)/sin_scroller.c $(ASMFILES) -o $(BUILD_PATH)/sin_scroller.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o

sin_scroller_multicolor:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) $(MULTICFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/sin_scroller.c $(ASMFILES) \
	-o $(BUILD_PATH)/sin_scroller_multicolor.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o

    
sin_scroller_expand_x:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) $(EXPANDXCFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/sin_scroller.c $(ASMFILES) \
	-o $(BUILD_PATH)/sin_scroller_expand_x.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o    

sin_scroller_multicolor_expand_x:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) $(EXPANDXCFG) $(MULTICFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/sin_scroller.c $(ASMFILES) \
	-o $(BUILD_PATH)/sin_scroller_multicolor_expand_x.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o 
    
sin_scroller_expand_y:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) $(EXPANDYCFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/sin_scroller.c $(ASMFILES) \
	-o $(BUILD_PATH)/sin_scroller_expand_y.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o   

sin_scroller_music:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYSIDCFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	--asm-define MUSIC_CODE=1 \
	$(SID_PATH)/sid.s \
	$(DEMOS_PATH)/sin_scroller.c $(ASMFILES) \
	-o $(BUILD_PATH)/sin_scroller_music.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o
	rm $(SID_PATH)/*.o        
    
sin_scroller_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) \
	--asm-define MAXSPR=16 \
	--asm-define FAST_MODE=1 \
	$(SOURCE_PATH)/generic_multiplexer.s \
	$(DEMOS_PATH)/sin_scroller.c \
	$(GRAPHICS_PATH)/graphics.s \
	-o $(BUILD_PATH)/sin_scroller_c128.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o

sin_scroller_music_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYSIDC128CFG) \
	--asm-define MAXSPR=16 \
	--asm-define FAST_MODE=1 \
	--asm-define MUSIC_CODE=1 \
	$(DEMOS_PATH)/sin_scroller.c \
	$(SOURCE_PATH)/generic_multiplexer.s \
	$(SID_PATH)/sid_at_2400.s \
    $(GRAPHICS_PATH)/graphics.s \
	-o $(BUILD_PATH)/sin_scroller_music_c128.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(SID_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o   


all: some_sprites many_sprites sin_scroller sin_scroller_multicolor sin_scroller_expand_x sin_scroller_multicolor_expand_x sin_scroller_expand_y sin_scroller_music sin_scroller_c128 sin_scroller_music_c128


clean:
	rm -rf *.prg
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(DEMOS_PATH)/*.o
	rm -rf ./build/*
	rm -rf main.s

    
many_sprites_debug:
	$(CC65_PATH)$(MYCC65) $(MYDEBUGCCFLAGS) -D_NUMBER_OF_SPRITES_=18 -D_SPRITE_SEPARATION_=25 \
	$(DEMOS_PATH)/many_sprites.c -o $(DEMOS_PATH)/many_sprites.s
	$(CC65_PATH)$(MYCL65) $(MYDEBUGCCFLAGS) $(MYCFG) --asm-define DEBUG=1 --asm-define MAXSPR=18 \
	$(DEMOS_PATH)/many_sprites.s $(ASMFILES) \
	-o $(BUILD_PATH)/many_sprites_debug.prg
	rm $(DEMOS_PATH)/many_sprites.s
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o
    
 
sin_scroller_debug:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYSIDCFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	--asm-define MUSIC_CODE=1 \
	--asm-define DEBUG=1 \
	$(SID_PATH)/sid.s \
	$(DEMOS_PATH)/sin_scroller.c $(ASMFILES) \
	-o $(BUILD_PATH)/sin_scroller_debug.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o
	rm $(SID_PATH)/*.o    

raster_split_test:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define MAXSPR=16 \
	--asm-define STANDARD_IRQ=1 \
	$(DEMOS_PATH)/raster_split_test.c \
	$(SOURCE_PATH)/raster_split.s \
	$(GRAPHICS_PATH)/graphics.s \
	-o $(BUILD_PATH)/raster_split.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o     
 

raster_split_test_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) \
	--asm-define MAXSPR=16 \
	-DNO_INPUT \
	$(DEMOS_PATH)/raster_split_test.c \
	$(SOURCE_PATH)/raster_split.s \
	$(GRAPHICS_PATH)/graphics.s \
	-o $(BUILD_PATH)/raster_split_c128.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o     
    

raster_split_hi_x_test:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define MAXSPR=16 \
	--asm-define STANDARD_IRQ=1 \
	$(DEMOS_PATH)/raster_split_hi_x_test.c \
	$(SOURCE_PATH)/raster_split_hi_x.s \
	$(GRAPHICS_PATH)/graphics.s \
	-o $(BUILD_PATH)/raster_split_hi_x.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o     
 

raster_split_hi_x_test_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) \
	--asm-define MAXSPR=16 \
	-DNO_INPUT \
	$(DEMOS_PATH)/raster_split_hi_x_test.c \
	$(SOURCE_PATH)/raster_split_hi_x.s \
	$(GRAPHICS_PATH)/graphics.s \
	-o $(BUILD_PATH)/raster_split_hi_x_c128.prg
	rm $(DEMOS_PATH)/*.o
	rm $(SOURCE_PATH)/*.o
	rm $(GRAPHICS_PATH)/*.o         

    
debug: many_sprites_debug sin_scroller_debug raster_split_hi_x_test raster_split_hi_x_test_c128

