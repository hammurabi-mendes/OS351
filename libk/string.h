/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * License: BSD 2-clause
 *
 * For copyright and credits, please refer to the corresponding files
 * in the project's root directory.
 */
#ifndef STRING_H
#define STRING_H

extern void *memcpy(void *destination, const void *source, int count);
extern void *memset(void *destination, unsigned char value, int count);
extern int strlen(const char *str);

#endif /* STRING_H */
