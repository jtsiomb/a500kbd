#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ps2kbd.h"
#include "defs.h"
#include "timer.h"

#define TIMEOUT	100

#define BUF_SZ	32
#define BUF_IDX_MASK	(BUF_SZ - 1)
#define NEXT_IDX(x) (((x) + 1) & BUF_IDX_MASK)
static volatile unsigned char keybuf[BUF_SZ];
static volatile unsigned char key_rd, key_wr;

static volatile int send_nbits, wait_ack;
static volatile unsigned char send_val, send_par;

static void abort_send(void);

int ps2write(unsigned char c)
{
	cli();
	send_nbits = 10;
	send_val = c;
	send_par = 0;	/* init to 0, will be calculated while sending */

	/* inhibit transmission, hold at least 100us */
	PORTD &= ~PCLK_BIT;
	DDRD |= PCLK_BIT;
	EIFR |= (1 << INTF0);	/* clear pending interrupt due to clock transition */
	sei();
	_delay_us(100);

	/* RTS by data low & release clock (this counts as start bit?) */
	PORTD &= ~PDATA_BIT;
	DDRD |= PDATA_BIT;
	DDRD &= ~PCLK_BIT;

	reset_timer();
	while(send_nbits > 0) {
		if(get_msec() > TIMEOUT) {
			abort_send();
			return -1;
		}
	}
	_delay_us(5);

	/* release data line and wait for ack */
	cli();
	wait_ack = 1;
	sei();
	DDRD &= ~PDATA_BIT;
	reset_timer();
	while(wait_ack) {
		if(get_msec() > TIMEOUT) {
			abort_send();
			return -1;
		}
	}
	return 0;
}

static void abort_send(void)
{
	cli();
	send_nbits = 0;
	wait_ack = 0;
	/* hold clock low for 100us */
	PORTD &= ~PCLK_BIT;
	DDRD |= PCLK_BIT;
	_delay_us(100);
	DDRD &= ~(PCLK_BIT | PDATA_BIT);
	EIFR |= (1 << INTF0);	/* clear pending interrupt */
	sei();
}

unsigned char ps2read(void)
{
	unsigned char key;

	while(key_rd == key_wr) {
	}

	cli();
	key = keybuf[key_rd];
	key_rd = NEXT_IDX(key_rd);
	sei();

	return key;
}

int ps2pending(void)
{
	return key_rd != key_wr;
}

int ps2wait(unsigned int timeout)
{
	reset_timer();
	while(key_rd == key_wr) {
		if(get_msec() >= timeout) return -1;
	}
	return 0;
}

#define PS2_ACK		0xfa
#define PS2_RESEND	0xfe
#define PS2_ECHO	0xee

int ps2setled(unsigned char state)
{
	unsigned char c;

	ps2write(0xed);
	reset_timer();
	while(!ps2pending()) {
		if(get_msec() >= TIMEOUT) return -1;
	}
	c = ps2read();
	/*printf("ps2setled 1st response: %x\r\n", (unsigned int)c);*/
	if(c != PS2_ACK) return -1;

	ps2write(state);
	reset_timer();
	while(!ps2pending()) {
		if(get_msec() >= TIMEOUT) return -1;
	}
	c = ps2read();
	/*printf("ps2setled 2nd response: %x\r\n", (unsigned int)c);*/
	if(c != PS2_ACK) return -1;

	return 0;
}

void ps2clearbuf(void)
{
	key_rd = key_wr;
}

ISR(INT0_vect)
{
	static unsigned char value, parity;
	/*static unsigned char valp;*/
	static int nbits;

	if(wait_ack) {
		if(!(PIND & PDATA_BIT)) {
			wait_ack = 0;
		}
		return;
	}

	if(send_nbits > 0) {
		--send_nbits;
		switch(send_nbits) {
		case 1:		/* parity bit */
			if(send_par & 1) {	/* odd number of ones: parity 0 */
				PORTD &= ~PDATA_BIT;
			} else {	/* even number of ones: parity 1 */
				PORTD |= PDATA_BIT;
			}
			break;
		case 0:	/* stop bit: 1 */
			PORTD |= PDATA_BIT;
			break;
		default:
			if(send_val & 1) {
				PORTD |= PDATA_BIT;
				++send_par;
			} else {
				PORTD &= ~PDATA_BIT;
			}
			send_val >>= 1;
		}

	} else {
		if(nbits > 0 && nbits < 9) {
			value >>= 1;
			if(PIND & PDATA_BIT) {
				value |= 0x80;
				parity ^= 1;
			}
		}/* else if(nbits == 9) {
			valp = (PIND >> PDATA) & 1;
		}*/
		if(++nbits >= 11) {
			nbits = 0;

			/* check parity */
			/*if((parity & 1) == (valp & 1)) {}*/
			keybuf[key_wr] = (unsigned char)value;
			key_wr = NEXT_IDX(key_wr);

			value = 0;
			parity = 0;
		}
	}
}
