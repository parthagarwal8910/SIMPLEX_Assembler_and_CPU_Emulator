CFLAGS = -std=c89 -pedantic -W -Wall -Wpointer-arith -Wwrite-strings -Wstrict-prototypes

all : asm emu

asm: asm.c
	gcc $(CFLAGS) asm.c -o asm

emu : emu.c
	gcc $(CFLAGS) emu.c -o emu

clean:
	rm -f asm emu
