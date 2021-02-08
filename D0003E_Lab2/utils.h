/*
 * utils.h
 *
 * Created: 2021-01-28 20:55:25
 *  Author: shirt
 */ 
#include <stdint-gcc.h>


void setbits(volatile uint8_t *ptr, int width, int start, uint8_t value);