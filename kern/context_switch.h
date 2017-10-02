#ifndef CONTEXT_SWITCH_H
#define CONTEXT_SWITCH_H

void prepare_context(void *stack, void *(*function)(void *), void *argument);
void switch_context(void **old_stack, void *new_stack);

#endif /* CONTEXT_SWITCH_H */
