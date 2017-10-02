#ifndef IO_H
#define IO_H

#include "stdint.h"

uint8_t inportb(uint16_t port);
void outportb(uint16_t port, uint8_t data);

#endif /* IO_H */
