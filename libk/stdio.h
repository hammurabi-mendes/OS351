#ifndef STDIO_H
#define STDIO_H

#define NULL ((void *) 0)

void reverse(char *buffer);
char* itoa(long int number, char* buffer, int base);
int puts(char *string);
int printf(char *format, ...);

#endif /* STDIO_H */
