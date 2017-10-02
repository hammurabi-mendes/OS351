/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * License: BSD 2-clause
 *
 * For copyright and credits, please refer to the corresponding files
 * in the project's root directory.
 */
#include "stdint.h"
#include "string.h"
#include "io.h"

#include "terminal.h"

#define BUFFER_POSITION(ROW, COL) ((ROW) * VGA_WIDTH + (COL))

static uint16_t *terminal_buffer = (uint16_t *) 0xB8000;

int position_col = 0, position_row = 0;

// Default color attribute is white on black
uint16_t color = (BLACK << 4) | WHITE;

void terminal_scroll(void) {
	// ASCII for 'space' + color parameters
	uint16_t blank = ' ' | (color << 8);

	for(int i = 0; i < VGA_HEIGHT - 1; i++) {
		for(int j = 0; j < VGA_WIDTH; j++) {
			terminal_buffer[BUFFER_POSITION(i, j)] = terminal_buffer[BUFFER_POSITION(i + 1, j)];
		}
	}

	for(int j = 0; j < VGA_WIDTH; j++) {
		terminal_buffer[BUFFER_POSITION(VGA_HEIGHT - 1, j)] = blank;
	}

	position_row = VGA_HEIGHT - 1;
}

void move_cursor(void) {
	unsigned int position = BUFFER_POSITION(position_row, position_col);

	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (unsigned char) (position & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (unsigned char) ((position >> 8) & 0xFF));
}

void terminal_initialize(void) {
	terminal_clear();
}

void terminal_setcolor(uint8_t foreground, uint8_t background) {
	color = ((background & 0x0F) << 4) | (foreground & 0x0F);
}

void terminal_write(char character) {
	if(character == '\b') {
		position_col--;
	}
	else if(character == '\t') {
		position_col += 8 - (position_col % 8);
	}
	else if(character == '\r') {
		position_col = 0;
	}
	else if(character == '\n') {
		position_col = 0;
		position_row++;
	}
	else {
		terminal_buffer[BUFFER_POSITION(position_row, position_col)] = character | (color << 8);
		position_col++;
	}

	if(position_col < 0) {
		if(position_row == 0) {
			position_col = 0;
		}
		else {
			position_row--;
			position_col = VGA_WIDTH - 1;
		}
	}

	if(position_col >= VGA_WIDTH) {
		position_col = 0;
		position_row++;
	}

	if(position_row >= VGA_HEIGHT) {
		terminal_scroll();
	}

	move_cursor();
}

void terminal_write_string(char *text, int size) {
	for(int i = 0; i < size; i++) {
		terminal_write(text[i]);
	}
}

void terminal_clear() {
	// ASCII for 'space' + color parameters
	uint16_t blank = ' ' | (color << 8);

	for(int i = 0; i < VGA_HEIGHT; i++) {
		for(int j = 0; j < VGA_WIDTH; j++) {
			terminal_buffer[BUFFER_POSITION(i, j)] = blank;
		}
	}

	position_col = 0;
	position_row = 0;

	move_cursor();
}
