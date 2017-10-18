#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include "timer.h"

#define PRESCL_256	4
/* 256 ticks per interrupt, 256 clock divisor */
#define TICKS_PER_SEC	(F_CPU / 256 / 256)

static volatile unsigned long ticks;

void init_timer(void)
{
	power_timer0_enable();

	TCCR0A = 0;
	TCCR0B = PRESCL_256;

	TIMSK0 |= (1 << TOIE0);	/* enable ovf intr. */
}

void reset_timer(void)
{
	ticks = 0;
}

unsigned long get_msec(void)
{
	return 1000 * ticks / TICKS_PER_SEC;
}

ISR(TIMER0_OVF_vect)
{
	++ticks;
}
