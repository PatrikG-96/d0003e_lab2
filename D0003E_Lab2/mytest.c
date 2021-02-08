#include "tinythreads.h"
#include <stdbool.h>
#include "utils.h"
#include "LCDDrivers.h"



int pp;
mutex m = MUTEX_INIT;

int is_prime(long i)
{
	long n = i-1;
	
	while (n>1)
	{
		if (i%n == 0)
		{
			return 0;
		}
		n--;
	}
	return 1;
}

void printAt(long num, int pos) {
	lock(&m);
    pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
	delay_loop(1000);
	pp++;
    writeChar( num % 10 + '0', pp);
	unlock(&m);
}

void delay_loop(int iter) {
	int i = 0;
	while (i < iter) {
		i++;
	}
}

void computePrimes(int pos) {
    long n;

    for(n = 1; ; n++) {
        if (is_prime(n)) {
            printAt(n, pos);
			//yield();
        }
    }
}

int main() {
	return 0;
    spawn(computePrimes, 0);
    computePrimes(3);
}
