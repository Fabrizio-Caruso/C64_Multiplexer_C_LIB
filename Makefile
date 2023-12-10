# Makefile 
CC65_PATH ?=
SOURCE_PATH ?= ./src
DEMOS_PATH ?= ./demos
GRAPHICS_PATH ?= ./graphics
SID_PATH ?= ./sid
BUILD_PATH ?= ./build


GEN_MPLX_ASM_FILES=$(SOURCE_PATH)/generic_multiplexer.s $(GRAPHICS_PATH)/graphics.s
SNOW_ASM_FILES=$(SOURCE_PATH)/generic_multiplexer.s $(GRAPHICS_PATH)/snow_flakes_graphics.s
SNOW_UDG_ASM_FILES=$(SOURCE_PATH)/generic_multiplexer.s $(GRAPHICS_PATH)/snow_flakes_graphics_udg.s
SNOW_UDG_ASM_FILES2=$(SOURCE_PATH)/generic_multiplexer.s $(GRAPHICS_PATH)/snow_flakes_graphics_udg2.s

COMMODORE_UDG_ASM_FILES=$(SOURCE_PATH)/generic_multiplexer.s $(GRAPHICS_PATH)/commodore_graphics_udg.s

RAS_SPLT_ASM_FILES=$(SOURCE_PATH)/raster_split.s $(GRAPHICS_PATH)/graphics.s


MYCCFLAGS=-t c64 -O -Cl
MYC128CCFLAGS=-t c128 -O -Cl
MYDEBUGCCFLAGS=-t c64
MULTICFG=--asm-define MULTICOLOR=1 -DMULTI_COLOR
EXPANDXCFG=--asm-define EXPANDX=1 -DEXPAND_X
EXPANDYCFG=--asm-define EXPANDY=1 -DEXPAND_Y


MYCFG=--config ./cfg/c64_multiplexer_gfx_at_2000.cfg 
MYC128CFG=--config ./cfg/c128_multiplexer_gfx_at_3000.cfg 
MYSIDCFG=--config ./cfg/c64_multiplexer_sid_at_1000_gfx_at_2000.cfg 
MYSIDCFG_ALT=--config ./cfg/c64_multiplexer_sid_at_1000_sprites_at_2800.cfg 
MYSIDC128CFG=--config ./cfg/c128_multiplexer_sid_at_2400_gfx_at_3000.cfg 


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

   
####################################################
# GENERIC MULTIPLEXER


# -- MANY_SPRITES TESTS C64
some_sprites: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define MAXSPR=26 -D_NUMBER_OF_SPRITES_=26 -D_SPRITE_SEPARATION_=24 \
	$(DEMOS_PATH)/generic_multiplexer/many_sprites_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o


many_sprites: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define MAXSPR=36 -D_NUMBER_OF_SPRITES_=36 -D_SPRITE_SEPARATION_=25 \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/generic_multiplexer/many_sprites_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o  


# SID AT $1000, SID_SIZE: $1000, GFX AT $2000, GFX_SIZE: $2000
many_flakes_old: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYSIDCFG) \
	--asm-define MAXSPR=22 -D_NUMBER_OF_SPRITES_=20 -D_SPRITE_SEPARATION_=30 \
	--asm-define FAST_MODE=1 \
	--asm-define MUSIC_CODE=1 \
	$(DEMOS_PATH)/generic_multiplexer/many_flakes_test.c \
	$(SNOW_ASM_FILES) \
	$(SID_PATH)/sid_Xmas.s \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o  

# SID AT $1000, SID_SIZE: $0C00, GFX AT $1C00, GFX_SIZE: $2000
# -m mapfile
many_flakes: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYSIDCFG_ALT) $(EXPANDXCFG) \
	--asm-define MAXSPR=20 -D_NUMBER_OF_SPRITES_=20  -D_SPRITE_SEPARATION_=30 \
	--asm-define FAST_MODE=1 \
	--asm-define MUSIC_CODE=1 \
	-DSPRITES_AT_2800 \
	$(DEMOS_PATH)/generic_multiplexer/many_flakes_test.c \
	$(SNOW_UDG_ASM_FILES) \
	$(SID_PATH)/sid_Xmas.s \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o 


many_flakes2: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYSIDCFG_ALT) $(EXPANDXCFG) \
	--asm-define MAXSPR=20 -D_NUMBER_OF_SPRITES_=20  -D_SPRITE_SEPARATION_=30 \
	--asm-define MUSIC_CODE=1 \
	--asm-define MULTICOLOR=1 \
	-DSPRITES_AT_2800 \
	$(DEMOS_PATH)/generic_multiplexer/many_flakes2_test.c \
	$(SNOW_UDG_ASM_FILES2) \
	$(SID_PATH)/sid_Xmas.s \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o 


# 	--asm-define DEBUG=1
happy_new_year: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYSIDCFG_ALT) $(EXPANDXCFG) \
	--asm-define MAXSPR=20 -D_NUMBER_OF_SPRITES_=20  -D_SPRITE_SEPARATION_=30 \
	--asm-define MUSIC_CODE=1 \
	--asm-define MULTICOLOR=1 \
	-DSPRITES_AT_2800 \
	$(DEMOS_PATH)/generic_multiplexer/many_flakes2_test.c \
	$(SNOW_UDG_ASM_FILES2) \
	$(SID_PATH)/sid_Xmas.s \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o 


# SID AT $1000, SID_SIZE: $0C00, GFX AT $1C00, GFX_SIZE: $2000
# -m mapfile
commodore: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYSIDCFG_ALT) \
	--asm-define MAXSPR=28 -D_NUMBER_OF_SPRITES_=28  -D_SPRITE_SEPARATION_=30 \
	--asm-define FAST_MODE=1 \
	--asm-define MUSIC_CODE=1 \
	-DSPRITES_AT_2800 \
	$(DEMOS_PATH)/generic_multiplexer/commodore_test.c \
	$(COMMODORE_UDG_ASM_FILES) \
	$(SID_PATH)/Commodore_64.s \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o 


    
too_many_sprites: 
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define MAXSPR=43 -D_NUMBER_OF_SPRITES_=43 -D_SPRITE_SEPARATION_=25 \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/generic_multiplexer/many_sprites_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o    


# -- MANY_SPRITES TESTS C128
many_sprites_c128: 
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) \
	--asm-define USE_KERNAL=1 \
	--asm-define MAXSPR=36 -D_NUMBER_OF_SPRITES_=36 -D_SPRITE_SEPARATION_=25 \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/generic_multiplexer/many_sprites_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o     
   

many_sprites_tests: \
	some_sprites \
	many_sprites \
	too_many_sprites \
	many_sprites_c128


# -- SIN SCROLLER TESTS C64
sin_scroller:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define FAST_MODE=1 \
	--asm-define MAXSPR=16 \
	$(DEMOS_PATH)/generic_multiplexer/sin_scroller_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o


sin_scroller_multicolor:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) $(MULTICFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/generic_multiplexer/sin_scroller_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o

    
sin_scroller_expand_x:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) $(EXPANDXCFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/generic_multiplexer/sin_scroller_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o    


sin_scroller_multicolor_expand_x:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) $(EXPANDXCFG) $(MULTICFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/generic_multiplexer/sin_scroller_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o 

    
sin_scroller_expand_y:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) $(EXPANDYCFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	$(DEMOS_PATH)/generic_multiplexer/sin_scroller_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o   


sin_scroller_music:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYSIDCFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	--asm-define MUSIC_CODE=1 \
	$(SID_PATH)/sid.s \
	$(DEMOS_PATH)/generic_multiplexer/sin_scroller_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o
	rm -rf $(SID_PATH)/*.o        


# -- SIN SCROLLER TESTS C128   
sin_scroller_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) \
	--asm-define USE_KERNAL=1 \
	--asm-define MAXSPR=16 \
	--asm-define FAST_MODE=1 \
	$(GEN_MPLX_ASM_FILES) \
	$(DEMOS_PATH)/generic_multiplexer/sin_scroller_test.c \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o


sin_scroller_music_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYSIDC128CFG) \
	--asm-define USE_KERNAL=1 \
	--asm-define MAXSPR=16 \
	--asm-define FAST_MODE=1 \
	--asm-define MUSIC_CODE=1 \
	$(DEMOS_PATH)/generic_multiplexer/sin_scroller_test.c \
	$(SID_PATH)/sid_at_2400.s \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(SID_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o   


sin_scroller_tests: \
    sin_scroller \
	sin_scroller_multicolor \
	sin_scroller_expand_x \
	sin_scroller_multicolor_expand_x \
	sin_scroller_expand_y \
	sin_scroller_music \
	sin_scroller_c128 \
	sin_scroller_music_c128

commodore_tests: \
	commodore
	
many_flakes_tests: \
	many_flakes

generic_multiplexer_tests: \
	many_sprites_tests \
	sin_scroller_tests \
	commodore_tests \
	many_flakes_tests \
	happy_new_year


####################################################
# RASTER SPLIT 
raster_split:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define USE_KERNAL=1 \
	--asm-define MAXSPR=16 \
	--asm-define STANDARD_IRQ=1 \
	-DNO_INPUT \
	$(DEMOS_PATH)/raster_split/raster_split_test.c \
	$(RAS_SPLT_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/raster_split/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o     
 
free_sprite:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define USE_KERNAL=1 \
	--asm-define STANDARD_IRQ=1 \
	--asm-define FREE_SPRITES=1 \
	-DFREE_SPRITES=1 \
	$(DEMOS_PATH)/raster_split/free_sprite_test.c \
	$(RAS_SPLT_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/raster_split/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o  


raster_split_no_input_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) \
	--asm-define USE_KERNAL=1 \
	--asm-define MAXSPR=16 \
	$(DEMOS_PATH)/raster_split/raster_split_test.c \
	$(RAS_SPLT_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/raster_split/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o     


raster_split_c128:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) \
	--asm-define USE_KERNAL=1 \
	--asm-define MAXSPR=16 \
	--asm-define STANDARD_IRQ=1 \
	$(DEMOS_PATH)/raster_split/raster_split_test.c \
	$(RAS_SPLT_ASM_FILES) \
	-o $(BUILD_PATH)/$@.prg
	rm -rf $(DEMOS_PATH)/raster_split/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o 


raster_split_tests: \
	raster_split \
	raster_split_no_input_c128 \
	raster_split_c128


####################################################
# BASIC 
raster_split_basic:
	$(CC65_PATH)ca65$(EXEEXT) -DBASIC -DBASE=0xC000 -D__C64__ \
	$(SOURCE_PATH)/raster_split.s \
	-o $(BUILD_PATH)/raster_split_basic.o
	$(CC65_PATH)$(MYCL65) $(BUILD_PATH)/$@.o --target none \
	--start-addr 0xC000 \
	-o $(BUILD_PATH)/raster_split_basic.prg


# - Start: SYS49152 (try twice) TODO: Why twice?
# - Enable all sprites: POKE 53269,255  
# - Sprite shape data in $2000 (Monitor: f 2000 203F ff)
basic_tests: \
	raster_split_basic

####################################################
clean:
	rm -rf *.prg
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(DEMOS_PATH)/*.o
	rm -rf ./build/*
	rm -rf main.s

   
all: \
	raster_split_tests \
	generic_multiplexer_tests

   
####################################################
# DEBUG 
many_sprites_debug:
	$(CC65_PATH)$(MYCC65) $(MYDEBUGCCFLAGS) -D_NUMBER_OF_SPRITES_=36 -D_SPRITE_SEPARATION_=25 \
	$(DEMOS_PATH)/generic_multiplexer/many_sprites_test.c -o $(DEMOS_PATH)/many_sprites.s
	$(CC65_PATH)$(MYCL65) $(MYDEBUGCCFLAGS) $(MYCFG) --asm-define DEBUG=1 --asm-define MAXSPR=36 \
	--asm-define USE_KERNAL=1 \
	$(DEMOS_PATH)/many_sprites.s \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/many_sprites_debug.prg
	rm -rf $(DEMOS_PATH)/many_sprites.s
	rm -rf $(DEMOS_PATH)/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o
    

many_sprites_c128_debug: 
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) \
	--asm-define MAXSPR=36 -D_NUMBER_OF_SPRITES_=36 -D_SPRITE_SEPARATION_=25 \
	--asm-define FAST_MODE=1 \
	--asm-define DEBUG=1 \
	--asm-define USE_KERNAL=1 \
	$(DEMOS_PATH)/generic_multiplexer/many_sprites_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/many_sprites_c128_debug.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o    
    
 
sin_scroller_debug:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYSIDCFG) \
	--asm-define MAXSPR=16  \
	--asm-define FAST_MODE=1 \
	--asm-define MUSIC_CODE=1 \
	--asm-define DEBUG=1 \
	--asm-define USE_KERNAL=1 \
	$(SID_PATH)/sid.s \
	$(DEMOS_PATH)/generic_multiplexer/sin_scroller_test.c \
	$(GEN_MPLX_ASM_FILES) \
	-o $(BUILD_PATH)/sin_scroller_debug.prg
	rm -rf $(DEMOS_PATH)/generic_multiplexer/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o
	rm -rf $(SID_PATH)/*.o    


raster_split_debug:
	$(CC65_PATH)$(MYCL65) $(MYCCFLAGS) $(MYCFG) \
	--asm-define MAXSPR=16 \
	--asm-define STANDARD_IRQ=1 \
	--asm-define DEBUG=1 \
	--asm-define USE_KERNAL=1 \
	$(DEMOS_PATH)/raster_split/raster_split_test.c \
	$(RAS_SPLT_ASM_FILES) \
	-o $(BUILD_PATH)/raster_split_debug.prg
	rm -rf $(DEMOS_PATH)/raster_split/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o   


raster_split_c128_debug:
	$(CC65_PATH)$(MYCL65) $(MYC128CCFLAGS) $(MYC128CFG) \
	--asm-define MAXSPR=16 \
	--asm-define DEBUG=1 \
	-DNO_INPUT \
	--asm-define USE_KERNAL=1 \
	$(DEMOS_PATH)/raster_split/raster_split_test.c \
	$(RAS_SPLT_ASM_FILES) \
	-o $(BUILD_PATH)/raster_split_c128_debug.prg
	rm -rf $(DEMOS_PATH)/raster_split/*.o
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(GRAPHICS_PATH)/*.o 


debug: \
	many_sprites_debug \
	sin_scroller_debug \
	raster_split_debug \
	raster_split_c128_debug






   