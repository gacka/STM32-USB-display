

#include "delay.h"

void _delay_ms( double __ms)
{
	double __tmp = ((F_CPU) / 172e3) * __ms;
        while (__tmp>0) __tmp--;
}


void _delay_us( double __us)
{
	double __tmp = ((F_CPU) / 172e6) * __us;
        while (__tmp>0) __tmp--;
}

