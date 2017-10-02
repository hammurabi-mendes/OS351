/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * License: BSD 2-clause
 *
 * For copyright and credits, please refer to the corresponding files
 * in the project's root directory.
 */
#ifndef TERMINAL_H
#define TERMINAL_H

#include "stdint.h"

#define VGA_HEIGHT 25
#define VGA_WIDTH  80

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define YELLOW_14
#define WHITE 15

void terminal_initialize();
void terminal_setcolor(uint8_t foreground, uint8_t background);
void terminal_write(char character);
void terminal_write_string(char *text, int size);
void terminal_clear();

#endif /* TERMINAL_H */
