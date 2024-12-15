all: endian.elf heap_mistakes.elf pointer_arith.elf

%.elf : %.c
	gcc -o $@ $^

clean:
	rm -f *.elf