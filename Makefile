all: pigeon-vm

pigeon-vm:
	gcc -o $@ src/main.c src/pigeon/pigeon.c src/asm/asm.c

clean:
	rm pigeon-vm
