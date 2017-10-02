/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * License: BSD 2-clause
 *
 * For copyright and credits, please refer to the corresponding files
 * in the project's root directory.
 */
#include "interrupts.h"

#include "stdio.h"
#include "string.h"
#include "terminal.h"
#include "io.h"

#define IDT_SIZE 256

struct idt_t_ {
	uint16_t base_00_15;
	uint16_t selector;
	uint8_t ignored;
	uint8_t flags;
	uint16_t base_16_31;
	uint32_t base_32_63;
	uint32_t reserved;
} __attribute__ ((packed));

typedef struct idt_t_ idt_t;

struct idt_ptr_t_ {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed));

typedef struct idt_ptr_t_ idt_ptr_t;

// IDT for interrupt handling
static idt_t idt[IDT_SIZE];

// Pointer to the IDT structure
static idt_ptr_t idt_pointer;

// Tick counter
static uint64_t ticks;
static uint64_t ticks_periodicity;

// Remap IRQs 0-15 to interrupts 32-47
int irq_remap(void) {
	// We are talking 8259 PIC here: http://wiki.osdev.org/PIC
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);

	return 0;
}

// Setup an entry in the IDT table
int interrupt_setup_entry(int number, size_t base, uint16_t selector, uint8_t flags) {
	idt_t *entry = &idt[number];

	entry->base_00_15 = (base & 0xFFFF);
	entry->base_16_31 = ((base >> 16) & 0xFFFF);
	entry->base_32_63 = ((base >> 32) & 0xFFFFFFFF);

	entry->selector = selector;
	entry->flags = flags;

	return 0;
}

int interrupts_init() {
	// Remap IRQs to ISRs 32...47
	irq_remap();

	memset(idt, 0, sizeof(idt_t) * IDT_SIZE);

	// This struct initialization syntax is allowed in C99
	idt_pointer = (idt_ptr_t) { .limit = (sizeof(idt_t) * IDT_SIZE) - 1, .base = (size_t) &idt };

	// 0x08 means: kernel-level routine (as opposed to user-level)
	// 0x8E means: 32-bit gate of an interrupt routine in kernel-level

	interrupt_setup_entry(0, (size_t) exception0, 0x08, 0x8E);
	interrupt_setup_entry(1, (size_t) exception1, 0x08, 0x8E);
	interrupt_setup_entry(2, (size_t) exception2, 0x08, 0x8E);
	interrupt_setup_entry(3, (size_t) exception3, 0x08, 0x8E);
	interrupt_setup_entry(4, (size_t) exception4, 0x08, 0x8E);
	interrupt_setup_entry(5, (size_t) exception5, 0x08, 0x8E);
	interrupt_setup_entry(6, (size_t) exception6, 0x08, 0x8E);
	interrupt_setup_entry(7, (size_t) exception7, 0x08, 0x8E);
	interrupt_setup_entry(8, (size_t) exception8, 0x08, 0x8E);
	interrupt_setup_entry(9, (size_t) exception9, 0x08, 0x8E);
	interrupt_setup_entry(10, (size_t) exception10, 0x08, 0x8E);
	interrupt_setup_entry(11, (size_t) exception11, 0x08, 0x8E);
	interrupt_setup_entry(12, (size_t) exception12, 0x08, 0x8E);
	interrupt_setup_entry(13, (size_t) exception13, 0x08, 0x8E);
	interrupt_setup_entry(14, (size_t) exception14, 0x08, 0x8E);
	interrupt_setup_entry(15, (size_t) exception15, 0x08, 0x8E);
	interrupt_setup_entry(16, (size_t) exception16, 0x08, 0x8E);
	interrupt_setup_entry(17, (size_t) exception17, 0x08, 0x8E);
	interrupt_setup_entry(18, (size_t) exception18, 0x08, 0x8E);
	interrupt_setup_entry(19, (size_t) exception19, 0x08, 0x8E);
	interrupt_setup_entry(20, (size_t) exception20, 0x08, 0x8E);
	interrupt_setup_entry(21, (size_t) exception21, 0x08, 0x8E);
	interrupt_setup_entry(22, (size_t) exception22, 0x08, 0x8E);
	interrupt_setup_entry(23, (size_t) exception23, 0x08, 0x8E);
	interrupt_setup_entry(24, (size_t) exception24, 0x08, 0x8E);
	interrupt_setup_entry(25, (size_t) exception25, 0x08, 0x8E);
	interrupt_setup_entry(26, (size_t) exception26, 0x08, 0x8E);
	interrupt_setup_entry(27, (size_t) exception27, 0x08, 0x8E);
	interrupt_setup_entry(28, (size_t) exception28, 0x08, 0x8E);
	interrupt_setup_entry(29, (size_t) exception29, 0x08, 0x8E);
	interrupt_setup_entry(30, (size_t) exception30, 0x08, 0x8E);
	interrupt_setup_entry(31, (size_t) exception31, 0x08, 0x8E);

	interrupt_setup_entry(32, (size_t) irq0, 0x08, 0x8E);
	interrupt_setup_entry(33, (size_t) irq1, 0x08, 0x8E);
	interrupt_setup_entry(34, (size_t) irq2, 0x08, 0x8E);
	interrupt_setup_entry(35, (size_t) irq3, 0x08, 0x8E);
	interrupt_setup_entry(36, (size_t) irq4, 0x08, 0x8E);
	interrupt_setup_entry(37, (size_t) irq5, 0x08, 0x8E);
	interrupt_setup_entry(38, (size_t) irq6, 0x08, 0x8E);
	interrupt_setup_entry(39, (size_t) irq7, 0x08, 0x8E);
	interrupt_setup_entry(40, (size_t) irq8, 0x08, 0x8E);
	interrupt_setup_entry(41, (size_t) irq9, 0x08, 0x8E);
	interrupt_setup_entry(42, (size_t) irq10, 0x08, 0x8E);
	interrupt_setup_entry(43, (size_t) irq11, 0x08, 0x8E);
	interrupt_setup_entry(44, (size_t) irq12, 0x08, 0x8E);
	interrupt_setup_entry(45, (size_t) irq13, 0x08, 0x8E);
	interrupt_setup_entry(46, (size_t) irq14, 0x08, 0x8E);
	interrupt_setup_entry(47, (size_t) irq15, 0x08, 0x8E);

	load_idt(&idt_pointer);

	enable_interrupts();

	return 0;
}

int handle_exception_context(interrupt_state_t *state) {
	// If you want different functions for different exceptions,
	// just check your interrupt_number field and decide which function to jump to.

	printf("Exception %d\n", state->interrupt_number);

	// Default: print registers
	print_interrupt_state(state);

	return 0;
}

int handle_irq_context(interrupt_state_t *state) {
	// If you want different functions for different exceptions,
	// just check your interrupt_number field and decide which function to jump to.

	// Timer
	if(state->interrupt_number == 32) {
		ticks++;

		if(ticks % ticks_periodicity == 0) {
			printf("Tick!\n");
		}
	}
	else {
		printf("IRQ %d\n", state->interrupt_number - 32);

		// Default: print registers
		print_interrupt_state(state);
	}

	// Signals the PICs for the END-OF-INTERRUPT

	// If the interrupt came from the slave PIC
	if(state->interrupt_number >= 40) {
		outportb(0xA0, 0x20); // Reset
	}

	// If the interrupt came from the master PIC
	outportb(0x20, 0x20); // Reset

	return 0;
}

void print_interrupt_state(interrupt_state_t *state) {
	printf("ss = %x, ", state->ss);
	printf("rsp = %x, ", state->rsp);
	printf("rflags = %x, ", state->rflags);
	printf("cs = %x, ", state->cs);
	printf("rip = %x\n", state->rip);
	printf("error_# = %l\n", state->error_number);
	printf("interrupt_# = %l, ", state->interrupt_number);
	printf("rax = %x, ", state->rax);
	printf("rcx = %x, ", state->rcx);
	printf("rdx = %x, ", state->rdx);
	printf("rbx = %x, ", state->rbx);
	printf("[BOGUS] rsp = %x, ", state->bogus_rsp);
	printf("rbp = %x, ", state->rbp);
	printf("rsi = %x, ", state->rsi);
	printf("rdi = %x, ", state->rdi);
	printf("r8 = %x, ", state->r8);
	printf("r9 = %x, ", state->r9);
	printf("r10 = %x, ", state->r10);
	printf("r11 = %x, ", state->r11);
	printf("r12 = %x, ", state->r12);
	printf("r13 = %x, ", state->r13);
	printf("r14 = %x, ", state->r14);
	printf("r15 = %x\n", state->r15);
}

void start_timer(int ms) {
	ticks_periodicity = ms;

	// The hardware will generate (1193180 / divisor) interrupts/second
	// Setting 1193180/1000 means firing it every 1000 times/second = 1ms
	uint32_t divisor = 1193180 / 1000;

	// Send the command byte
	outportb(0x43, 0x36);

	// Divisor has to be sent in two parts: low and high, in that order
	uint8_t low = (uint8_t) (divisor & 0xFF);
	uint8_t high = (uint8_t) ((divisor >> 8) & 0xFF);

	// Send the data fields
	outportb(0x40, low);
	outportb(0x40, high);
}
