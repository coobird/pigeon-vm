TARGET = pigeon-vm
OBJS = src/main.o src/pigeon/pigeon.o src/asm/asm.o
BUILT = $(TARGET) $(OBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $^ 

%.o: %.c
	gcc -o $@ -c $<

clean:
	rm -f $(BUILT)
