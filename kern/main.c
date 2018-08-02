/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * License: BSD 2-clause
 *
 * For copyright and credits, please refer to the corresponding files
 * in the project's root directory.
 */
#include "stdio.h"
#include "string.h"

#include "terminal.h"
#include "interrupts.h"
#include "threads.h"

char stack[12288] __attribute__((aligned(16)));

void *function1(void *argument) {
    while(1) {
        printf("Hola! ");
        thread_yield();
    }

    return NULL;
}

void *function2(void *argument) {
    while(1) {
        printf("Aloha! ");
        thread_yield();
    }

    return NULL;
}

void loop() {
    while(1) {
        printf("Loop ");
    }
}

int main(void) {
    terminal_clear();
    printf("Hello World!");

    interrupts_init();
    start_timer(1000);

    threading_init();
    int tid1 = thread_create(function1, NULL);
    int tid2 = thread_create(function2, NULL);

    thread_join(tid1);
    thread_join(tid2);

    loop();
}
