all: execExample.bin forkExample.bin

execExample.bin: execExample.c
	gcc execExample.c -o execExample.bin

forkExample.bin: forkExample.c
	gcc forkExample.c -o forkExample.bin

clean:
	rm -f *.bin
