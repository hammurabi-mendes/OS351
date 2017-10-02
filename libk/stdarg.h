/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * License: BSD 2-clause
 *
 * For copyright and credits, please refer to the corresponding files
 * in the project's root directory.
 */
#ifndef STDARG_H
#define STDARG_H

// Possible naive implementation of these compiler-built-in functions:
// va_list:
//   #define va_list void *
// va_start(X, variable):
//   First six arguments are passed in registers rdi, rsi, rdx, rcx, r8, and r9, and the
//   remaining in the stack. Get the position of the last known argument X, and move to the
//   following argument in the register list. If you are in the stack, sum sizeof(variable)
//   to the address of the current argument in order to find the next argument.
// va_arg(X, type):
//   Do the same as above, but after doing so, cast the result as a pointer of type "type",
//   and get its value using the * operator.
// va_end(X)
//   That can actually be a no-op.

#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end

#endif /* STDARG_H */
