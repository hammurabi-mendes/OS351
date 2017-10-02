#include "string.h"

void *memcpy(void *destination, const void *source, int count) {
	unsigned char *d = (unsigned char *) destination;
	unsigned char *s = (unsigned char *) source;

	for(int i = 0; i < count; i++) {
		d[i] = s[i];
	}

	return destination;
}

void *memset(void *destination, unsigned char value, int count) {
	unsigned char *d = (unsigned char *) destination;

	for(int i = 0; i < count; i++) {
		d[i] = value;
	}

	return destination;
}

int strlen(const char *string) {
	int counter = 0;

	for(int i = 0; string[i] != '\0'; i++) {
		counter++;	
	}

	return counter;
}
