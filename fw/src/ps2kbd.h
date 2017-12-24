#ifndef PS2KBD_H_
#define PS2KBD_H_

enum {
	PS2LED_SCRLK = 1,
	PS2LED_NUMLK = 2,
	PS2LED_CAPSLK = 4
};

int ps2write(unsigned char c);
unsigned char ps2read(void);
int ps2pending(void);
int ps2wait(unsigned int timeout);
void ps2clearbuf(void);

int ps2setled(unsigned char state);

#endif	/* PS2KBD_H_ */
