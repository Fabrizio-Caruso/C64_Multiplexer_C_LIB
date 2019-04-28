
# CC65_PATH ?= /cygdrive/c/cc65-snapshot-win32/bin/
CC65_PATH ?= 


CCFLAGS=-t c64 --cpu 6502X --add-source
LDFLAGS=-l
OPTFLAGS=--static-locals -Osir
ASMFILES=irq_asm.s

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

test:
	$(CC65_PATH)$(MYCL65)$ -t c64 -Cl irq_test.c $(ASMFILES) -o irq_test.prg

all:
	$(CC65_PATH)$(MYCC65)$ $(CCFLAGS) --static-locals irq_test.c -o irq_test.s
	$(CC65_PATH)$(MYCL65)$ $(CCFLAGS) irq_test.s $(ASMFILES) -o irq_test.prg
