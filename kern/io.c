#include "io.h"

// Inline assembly adapted from Bran's Kernel Development tutorial

uint8_t inportb(uint16_t port) {
	unsigned char byte;

	__asm__ __volatile__("inb %1, %0" : "=a" (byte) : "dN" (port));

	return byte;
}

void outportb(uint16_t port, uint8_t data) {
	__asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (data));
}
