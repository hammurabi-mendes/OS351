/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * License: BSD 2-clause
 *
 * For copyright and credits, please refer to the corresponding files
 * in the project's root directory.
 */
#ifndef STDIO_H
#define STDIO_H

#define NULL ((void *) 0)

void reverse(char *buffer);
char* itoa(long int number, char* buffer, int base);
int puts(char *string);
int printf(char *format, ...);

#endif /* STDIO_H */
