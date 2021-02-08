/*
 * LCDDrivers.c
 *
 * Created: 2021-02-03 10:27:42
 *  Author: shirt
 */ 
#include <avr/io.h>

#define LCDADDRESS 0xEC


static const int sccDigits[10] = {0x1551, 0x0110, 0x1e11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x0B51};

void initLCD()
{
	
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// Using asynchronous clock, setting segments to 25, setting duty to 1/4
	LCDCRB = (1<<LCDCS) | (1<<LCDMUX0) | (1<<LCDMUX1) | (1<<LCDPM2)| (1<<LCDPM1) | (1<<LCDPM0);
	
	// Prescaler set to N = 16, Clock divide D=8
	LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
	
	//Drive time set to 300 microseconds
	LCDCCR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
	
	// Low power waveform, enabling LCD
	LCDCRA = (1<<LCDEN) | (1<<LCDAB);
}

void writeChar(char ch, int pos)
{
	// If out of bounds, do nothing
	if (pos > 5)
	{
		return;
	}
	
	int offset = (pos % 2); //offset within a column
	int column = pos / 2;
	uint8_t reset_mask;
	uint8_t digit_nibble;
	uint8_t cleared_nibble;
	uint16_t digit;
	uint8_t *addr = (uint8_t*) LCDADDRESS + column;
	
	// if ch is not a digit, empty the position instead
	if (isdigit(ch))
	{
		uint8_t index = ch - '0';	// Convert the character to an integer
		digit = sccDigits[index];	// Use integer to index the SCC array
	}
	else
	{
		digit = 0x0000;
	}
	
	for(int i = 0; i < 4; i++)
	{
		reset_mask = (i>0) ? 0b0000 : 0b0110;					// For LCDDR 0/1/2, preserve unused bits 1 and 2, otherwise preserve no bits
		cleared_nibble = (*addr << 4*offset) & reset_mask;	// Use reset_mask to clear the relevant nibble in memory
		digit_nibble = (digit) & 0x0f;						// Mask upper 4 bits
		digit_nibble = digit_nibble | cleared_nibble;		// Combine the cleared nibble with masked nibble to preserve bits not used
		setbits(addr, 4, 4*offset, digit_nibble);
		digit = digit >> 4;									// Shift right to get to the next nibble of the digit
		addr+=5;											// Increment the address, makes sure we use LCDDRX, LCDDRX+5, LCDDRX+10, LCDDRX+15
	}
	
}

void writeLong(long i)
{
	
	long x = i;
	int j = 5;	// max index
	char digits[6];
	
	// Iterate downwards
	while (x>0 && j >= 0)
	{
		int t = x % 10;
		char ch = t + '0';
		digits[j] = ch;
		j--;
		x = x/10;
	}
	
	for (int i = 0; i < 6; i++)
	{
		writeChar(digits[i],i);
	}
	
}