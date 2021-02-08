/*
 * utils.c
 *
 * Created: 2021-01-28 20:55:13
 *  Author: shirt
 */ 
#include <stdint-gcc.h>


void setbits(volatile uint8_t *ptr, int width, int start, uint8_t value)
{
	uint8_t mask = ~(~0 << width) << start;
	uint8_t data = (value << start) & mask;
	*ptr = *ptr & ~mask;
	*ptr = *ptr | data;
}