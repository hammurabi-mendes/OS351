OBJECTS = main.o io.o terminal.o interrupts.o interrupt_handlers.o threads.o context_switch.o

all: $(OBJECTS)

interrupt_handlers.o: interrupt_handlers.s
	$(AS) $(ASFLAGS) interrupt_handlers.s

context_switch.o: context_switch.s
	$(AS) $(ASFLAGS) context_switch.s

clean:
	rm -f *.o
