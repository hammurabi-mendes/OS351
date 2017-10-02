#ifndef STRING_H
#define STRING_H

extern void *memcpy(void *destination, const void *source, int count);
extern void *memset(void *destination, unsigned char value, int count);
extern int strlen(const char *str);

#endif /* STRING_H */
