



#ifndef _UTIL_DELAY_H_
#define _UTIL_DELAY_H_ 1



#define F_CPU 24000000
void _delay_ms( double __ms);
__inline void _delayInline_ms( double __ms)
{
	double __tmp = ((F_CPU) / 172e3) * __ms;
    while (__tmp>0) __tmp--;
}

void _delay_us( double __us);

#endif /* _UTIL_DELAY_H_ */
