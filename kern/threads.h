#ifndef THREADS_H
#define THREADS_H

#include "stdint.h"

#define STATE_ACTIVE   0
#define STATE_BLOCKED  1
#define STATE_FINISHED 2
#define STATE_INVALID  3

struct tcb_t_ {
	int number;
	void *stack;

	void *(*function)(void *);
	void *argument;
	void *return_value;

	int state;
	int joiner_thread_number;
};

typedef struct tcb_t_ tcb_t;

extern tcb_t *current_thread_context;

void threading_init(void);

int thread_create(void *(*function)(void *), void *argument);
int thread_yield(void);
void thread_join(int target_thread_number);
void thread_exit(void *return_value);

#endif /* THREADS_H */
