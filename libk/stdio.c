#include "stdarg.h"

#include "stdio.h"
#include "string.h"
#include "terminal.h"

void reverse(char *buffer) {
	int size = strlen(buffer);

	char temporary;

	for(int i = 0; i < size / 2;  i++) {
		temporary = buffer[i];
		buffer[i] = buffer[size - 1 - i];
		buffer[size - 1 - i] = temporary;
	}
}

char* itoa(long int number, char* buffer, int base) {
	int position = 0;

	long int current;

	if(number < 0) {
		current = -number;
	}
	else {
		current = number;
	}

	long int rest = 0;

	do {
		rest = current % base;
		current = current / base;

		if(rest < 10) {
			buffer[position++] = rest + '0';
		}
		else {
			buffer[position++] = rest - 10 + 'A';
		}
	} while(current > 0);

	if(number < 0) {
		buffer[position++] = '-';
	}

	buffer[position] = '\0';

	reverse(buffer);

	return buffer;
}

int puts(char *string) {
	for(char *current = string; *current != '\0'; current++) {
		terminal_write(*current);
	}

	return 1;
}

int printf(char *format, ...) {
	va_list arguments;

	va_start(arguments, format);

	char *current = format;

	int d;
	unsigned int u;
	unsigned int x;
	long int l;

	char buffer[128];

	for(current = format; *current != '\0'; current++) {
		if(*current != '%') {
			terminal_write(*current);
			continue;
		}

		current++;

		switch(*current) {
		case 'd':
			d = va_arg(arguments, int);
			itoa(d, buffer, 10);
			puts(buffer);
			break;
		case 'u':
			u = va_arg(arguments, unsigned int);
			itoa(u, buffer, 10);
			puts(buffer);
			break;
		case 'x':
			x = va_arg(arguments, unsigned int);
			puts("0x");
			itoa(x, buffer, 16);
			puts(buffer);
			break;
		case 'l':
			l = va_arg(arguments, long int);
			itoa(l, buffer, 10);
			puts(buffer);
			break;
		case '%':
			terminal_write('%');
			break;
		}
	}

	va_end(arguments);

	return 1;
}
