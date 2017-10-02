#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "stdint.h"

#include "interrupt_handlers.h"

int interrupts_init();

struct interrupt_state_t_ {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rbp;
	uint64_t bogus_rsp;
	uint64_t rbx;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rax;

	uint64_t interrupt_number;
	uint64_t error_number;

	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__ ((packed));

typedef struct interrupt_state_t_ interrupt_state_t;

void print_interrupt_state(interrupt_state_t *state);

void start_timer(int ms);

#endif /* INTERRUPTS_H */
