include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
STATICLIBS=./libc/libc.a

all: stdlib $(MODULE)

$(MODULE): $(SOURCES) others.a
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld _loader.c $(SOURCES) others.a ./libc/libc.a -o ../$(MODULE) -I./libc/include
	$(GCC) $(GCCFLAGS) -I./libc/include -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(OBJECTS_ASM) $(SOURCES)  others.a ./libc/libc.a -o ../0000-sampleCodeModule.elf
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

others.a: $(OBJECTS_ASM)
	$(AR) rvs others.a $(OBJECTS_ASM)

stdlib:
	cd libc; make all

clean: stdclean
	rm -rf *.o others.a

stdclean:
	cd libc; make clean

.PHONY: all clean print
