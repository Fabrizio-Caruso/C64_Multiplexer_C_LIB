# Makefile 
CC65_PATH ?= /cygdrive/c/cc65-snapshot-win32/bin/

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


MYCC65 ?= cl65$(EXEEXT) $(INCLUDE_OPTS) 

test:
	$(CC65_PATH)$(MYCC65) -O -t c64 main.c -o main.prg
	rm main.o