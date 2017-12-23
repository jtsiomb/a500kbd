#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "amigakb.h"
#include "defs.h"
#include "timer.h"

#define TIMEOUT_MSEC	143

static void resync(void);

static unsigned char prev_keycode = 0xff;
static unsigned char capslk;

void amikb_sendkey(unsigned char keycode, int press)
{
	int i;

	if(keycode == 0x62) {
		/* caps lock doesn't get a key release event when the key is released
		 * but rather when the caps lock is toggled off again
		 */
		if(!press) return;

		capslk = ~capslk;
		press = capslk;
	}

	/* keycode bit transfer order: 6 5 4 3 2 1 0 7 (7 is pressed flag) */
	keycode = (keycode << 1) | (~press & 1);
	if(keycode == prev_keycode) return;
	prev_keycode = keycode;

	/* make sure we don't pulse the lines while grabbing control
	 * by first reinstating the pullups before changing direction
	 */
	PORTD |= ACLK_BIT | ADATA_BIT;
	DDRD |= ACLK_BIT | ADATA_BIT;

	/* pulse the data line and wait for about 100us */
	PORTD &= ~ADATA_BIT;
	_delay_us(20);
	PORTD |= ADATA_BIT;
	_delay_us(100);

	for(i=0; i<8; i++) {
		/* data line is inverted */
		if(keycode & 0x80) {
			PORTD &= ~ADATA_BIT;
		} else {
			PORTD |= ADATA_BIT;
		}
		keycode <<= 1;
		_delay_us(20);
		/* pulse the clock */
		cli();
		PORTD &= ~ACLK_BIT;
		EIFR |= (1 << INTF1);
		sei();
		_delay_us(20);
		PORTD |= ACLK_BIT;
		_delay_us(20);
	}

	/* similarly tristate first, then drop the pullups */
	DDRD &= ~(ACLK_BIT | ADATA_BIT);
	PORTD &= ~(ACLK_BIT | ADATA_BIT);

	/* wait for ack */
	reset_timer();
	while(PIND & ADATA_BIT) {
		if(get_msec() >= TIMEOUT_MSEC) {
			resync();
			break;
		}
	}
}

void amikb_reset(void)
{
	cli();
	PORTD &= ~ARST_BIT;
	DDRD |= ARST_BIT;
	_delay_ms(10);
	PORTD |= ARST_BIT;
	DDRD &= ~ARST_BIT;

	prev_keycode = 0xff;
	capslk = 0;
	sei();
}

static void resync(void)
{
	PORTD |= ACLK_BIT | ADATA_BIT;
	printf("lost sync\r\n");

	for(;;) {
		cli();
		DDRD |= ACLK_BIT | ADATA_BIT;

		PORTD &= ~ACLK_BIT;
		EIFR |= (1 << INTF1);	/* clear interrupt raised by the previous line */
		sei();
		_delay_us(20);
		PORTD |= ACLK_BIT;

		DDRD &= ~(ACLK_BIT | ADATA_BIT);

		reset_timer();
		while(get_msec() < TIMEOUT_MSEC) {
			if(!(PIND & ADATA_BIT)) {
				return;
			}
		}
	}
}

static void handle_cmd(unsigned char cmd)
{
	printf("amikbd got cmd: %x\r\n", (unsigned int)cmd);
}

ISR(INT1_vect)
{
	static unsigned char value;
	static int nbits;

	value <<= 1;
	if(PIND & ADATA_BIT) {
		value |= 1;
	}
	if(++nbits >= 8) {
		handle_cmd(value);
		nbits = 0;
		value = 0;
	}
}
