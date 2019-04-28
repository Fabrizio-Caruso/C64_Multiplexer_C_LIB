CCFLAGS=-t c64 --cpu 6502X --add-source
LDFLAGS=-l
OPTFLAGS=--static-locals -Osir
ASMFILES=irq_asm.s

all:
	cc65 $(CCFLAGS) --static-locals irq_test.c -o irq_test.s
	cl65 $(CCFLAGS) irq_test.s $(ASMFILES) -o irq_test.prg
