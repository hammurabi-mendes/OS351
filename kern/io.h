/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * License: BSD 2-clause
 *
 * For copyright and credits, please refer to the corresponding files
 * in the project's root directory.
 */
#ifndef IO_H
#define IO_H

#include "stdint.h"

uint8_t inportb(uint16_t port);
void outportb(uint16_t port, uint8_t data);

#endif /* IO_H */
