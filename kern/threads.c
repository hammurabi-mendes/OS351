/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * License: BSD 2-clause
 *
 * For copyright and credits, please refer to the corresponding files
 * in the project's root directory.
 */
#include "threads.h"

#include "stdio.h"
#include "string.h"
#include "interrupts.h"
#include "context_switch.h"

#define MAX_THREADS 128

#define STACK_SIZE 65536

static tcb_t thread_context[MAX_THREADS];

static char thread_stack[MAX_THREADS][STACK_SIZE];

tcb_t *current_thread_context = NULL;

extern void *kernel_stack_bot;
extern void *kernel_stack_top;

void threading_init(void) {
	for(int i = 0; i < MAX_THREADS; i++) {
		thread_context[i].number = i;
		thread_context[i].state = STATE_INVALID;
	}

	tcb_t *new_context = &thread_context[0];

	new_context->function = NULL;
	new_context->argument = NULL;
	new_context->return_value = NULL;

	new_context->state = STATE_ACTIVE;
	new_context->joiner_thread_number = -1;

	new_context->stack = NULL;

	current_thread_context = &thread_context[0];
}

void setup_context(tcb_t *context) {
	prepare_context(&(context->stack), context->function, context->argument);

	//printf("New context created:\n");
	//print_interrupt_state(context->stack);
}

int thread_create(void *(*function)(void *), void *argument) {
	int chosen_position = -1;

	int current_thread_number = current_thread_context->number;

	// For NUMBER_TASKS times, iterate cyclically in the thread_context array
	for(int i = 0; i < MAX_THREADS; i++) {
		int current_position = (current_thread_number + 1 + i) % MAX_THREADS;

		if(thread_context[current_position].state == STATE_INVALID) {
			chosen_position = current_position;
			break;
		}
	}

	if(chosen_position == -1) {
		return -1;
	}

	tcb_t *new_context = &thread_context[chosen_position];

	new_context->function = function;
	new_context->argument = argument;
	new_context->return_value = NULL;

	new_context->state = STATE_ACTIVE;
	new_context->joiner_thread_number = -1;

	// TODO: take the 16 off
	new_context->stack = &thread_stack[chosen_position][4096 - 16];

	setup_context(new_context);

	return 0;
}

void thread_switch(tcb_t *old_context, tcb_t *new_context) {
	current_thread_context = new_context;

	switch_context(&(old_context->stack), new_context->stack);
}

int thread_yield(void) {
	int current_thread_number = current_thread_context->number;
	int new_thread_number = current_thread_context->number;

	do {
		new_thread_number++;
		new_thread_number %= MAX_THREADS;;
	} while(thread_context[new_thread_number].state != STATE_ACTIVE);

	if(new_thread_number == current_thread_number) {
		return 0;
	}

	thread_switch(&thread_context[current_thread_number], &thread_context[new_thread_number]);

	return 1;
}

void thread_join(int target_thread_number) {
	tcb_t *target_thread_context = &thread_context[target_thread_number];

	current_thread_context->state = STATE_BLOCKED;

	target_thread_context->joiner_thread_number = current_thread_context->number;

	thread_switch(current_thread_context, target_thread_context);

	current_thread_context->state = STATE_ACTIVE;
}

void thread_exit(void *return_value) {
	current_thread_context->return_value = return_value;
	current_thread_context->state = STATE_FINISHED;

	if(current_thread_context->joiner_thread_number != -1) {
		tcb_t *target_thread_context = &thread_context[current_thread_context->joiner_thread_number];

		thread_switch(current_thread_context, target_thread_context);
	}
	else {
		thread_yield();
	}
}
