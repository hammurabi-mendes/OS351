OBJECTS = loader.o

all: $(OBJECTS)

loader.o: loader.s
	$(AS) $(ASFLAGS) loader.s

loader32.o: loader32.s
	$(AS) $(ASFLAGS) loader32.s

clean:
	rm -f *.o
