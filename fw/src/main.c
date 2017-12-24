#include <stdio.h>
#include <ctype.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "serial.h"
#include "scantbl.h"
#include "ps2kbd.h"
#include "amigakb.h"
#include "defs.h"
#include "timer.h"

enum {
	KF_BRK = 1,
	KF_EXT = 2,
	KF_EXT1 = 4,

	KF_CTRL		= 16,
	KF_LAMIGA	= 32,
	KF_RAMIGA	= 64
};
#define KF_TRANSIENT	0x0f
#define KF_STICKY		0xf0

#define RESET_WAIT 1000

static unsigned char led_state;

int main(void)
{
	unsigned int keyflags = 0;
	unsigned char c, keycode, prev_drvled = 0;
	int press;

	/* disable all pullups globally */
	MCUCR |= 1 << PUD;

	DDRD = 0;
	PORTD = 0;
	EIMSK = 0;	/* mask external interrupts */
	EICRA = (1 << ISC11) | (1 << ISC01);	/* falling edge interrupts */

	init_timer();

	/* initialize the UART and enable interrupts */
	init_serial(9600);
	sei();

	printf("PS/2 keyboard controller - John Tsiombikas <nuclear@member.fsf.org>\r\n");

	EIMSK = (1 << INT0) | (1 << INT1);	/* enable ps/2 clock interrupt */

	ps2setled(0);	/* start with all LEDs off */

	for(;;) {
		while(!ps2pending()) {
			unsigned char drvled = PIND & ADRVLED_BIT;
			if(drvled != prev_drvled) {
				prev_drvled = drvled;
				if(drvled) {
					led_state |= PS2LED_SCRLK;
				} else {
					led_state &= ~PS2LED_SCRLK;
				}
				ps2setled(led_state);
			}
		}

		c = ps2read();
		switch(c) {
		case 0xe0:	/* extended */
			keyflags |= KF_EXT;
			break;

		case 0xe1:	/* extended */
			keyflags |= KF_EXT1;
			break;

		case 0xf0:	/* break code */
			keyflags |= KF_BRK;
			break;

		default:
			press = !(keyflags & KF_BRK);

			keycode = 0xff;
			if(keyflags & KF_EXT) {
				if(c < KEYMAP_EXT_SIZE) {
					keycode = keymap_ext[(int)c];
				}
			} else if(keyflags & KF_EXT1) {
			} else {
				if(c < KEYMAP_NORMAL_SIZE) {
					keycode = keymap_normal[(int)c];
				}
			}

			switch(keycode) {
			case AMIKEY_CTRL:
				if(press)
					keyflags |= KF_CTRL;
				else
					keyflags &= ~KF_CTRL;
				break;

			case AMIKEY_LAMI:
				if(press)
					keyflags |= KF_LAMIGA;
				else
					keyflags &= ~KF_LAMIGA;
				break;

			case AMIKEY_RAMI:
				if(press)
					keyflags |= KF_RAMIGA;
				else
					keyflags &= ~KF_RAMIGA;
				break;

			default:
				break;
			}

			if((keyflags & (KF_CTRL | KF_RAMIGA | KF_LAMIGA)) == (KF_CTRL | KF_RAMIGA | KF_LAMIGA)) {
				printf("CTRL - AMIGA - AMIGA!\r\n");
				amikb_reset();

				keyflags = 0;

				reset_timer();
				while(get_msec() < RESET_WAIT);
				ps2clearbuf();

				break;
			}

			if(keycode != 0xff) {
				int press = ~keyflags & KF_BRK;
				amikb_sendkey(keycode, press);
				if(keycode == 0x62 && press) {
					led_state ^= PS2LED_CAPSLK;
					ps2setled(led_state);
				}
				/*printf("scancode %x -> [%s] amiga key %xh\r\n", (unsigned int)c, press ? "press" : "release", keycode);*/
			} else {
				printf("PS/2 unknown command or keycode: %x\r\n", (unsigned int)c);
			}
			keyflags &= ~KF_TRANSIENT;
		}
	}
	return 0;
}
