# Makefile 
CC65_PATH ?=
CCFLAGS=-t c64 --cpu 6502X --add-source
ASMFILES=multi_ca65_split.s

MYCCFLAGS=-t c64 -O -Cl
MYCFG=--config ./cfg/c64_multiplexer.cfg --asm-define USE_KERNAL=1

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


SOURCE_PATH := chase


MYCC65 ?= cc65$(EXEEXT) $(INCLUDE_OPTS) 
MYCL65 ?= cl65$(EXEEXT) $(INCLUDE_OPTS) 

multiplexer: 
	$(CC65_PATH)$(MYCL65)$ $(MYCCFLAGS) $(MYCFG) main.c $(ASMFILES) -o multiplexer.prg
	rm main.o
	rm multi_ca65_split.o

debug:
	$(CC65_PATH)$(MYCC65)$ $(CCFLAGS) main.c -o main.s
	$(CC65_PATH)$(MYCL65)$ $(CCFLAGS) $(MYCFG) --asm-define DEBUG=1 main.s $(ASMFILES) -o multiplexer_debug.prg
	rm main.o
	rm multi_ca65_split.o
    
all: multiplexer debug

clean:
	rm -rf *.prg
	rm -rf *.o