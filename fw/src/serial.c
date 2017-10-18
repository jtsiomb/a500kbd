#ifndef F_CPU
#ifdef XTAL
#define F_CPU	XTAL
#else
#warning "compiled for 1mhz internal rc osc. serial comms won't work"
#define F_CPU	1000000
#endif
#endif

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>

static int uart_send_char(char c, FILE *fp);
static int uart_get_char(FILE *fp);

#define BUF_SZ	16
#define BUF_IDX_MASK	(BUF_SZ - 1)
#define NEXT_IDX(x)	(((x) + 1) & BUF_IDX_MASK)
static char outbuf[BUF_SZ];
static volatile unsigned char out_rd, out_wr;
static char inbuf[BUF_SZ];
static volatile unsigned char in_rd, in_wr;

static FILE std_stream = FDEV_SETUP_STREAM(uart_send_char, uart_get_char, _FDEV_SETUP_RW);



void init_serial(long baud)
{
	unsigned int ubrr_val = F_CPU / 16 / baud - 1;

	power_usart0_enable();

	/* set baud generator timer reset value */
	UBRR0H = (unsigned char)(ubrr_val >> 8);
	UBRR0L = (unsigned char)ubrr_val;

	/* enable rx/tx and recv interrupt */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	/* set frame format: 8n1 */
	UCSR0C = 3 << UCSZ00;

	stdin = stdout = stderr = &std_stream;
}

int have_input(void)
{
	return in_wr != in_rd;
}

static int uart_send_char(char c, FILE *fp)
{
	/*int next;*/

	while((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = (unsigned char)c;
#if 0
	next = NEXT_IDX(out_wr);
	while(next == out_rd);

	outbuf[out_wr] = c;
	out_wr = next;

	/* enable the Tx data register empty interrupt */
	UCSR0B |= 1 << UDRIE0;
#endif
	return 0;
}

static int uart_get_char(FILE *fp)
{
	char c;

	while(in_rd == in_wr);

	c = inbuf[in_rd];
	in_rd = NEXT_IDX(in_rd);
	return c;
}

ISR(USART_RX_vect)
{
	char c = UDR0;

	inbuf[in_wr] = c;
	in_wr = NEXT_IDX(in_wr);
}

/* USART Tx data register empty (can send more data) */
ISR(USART_UDRE_vect)
{
	if(out_rd != out_wr) {
		UDR0 = outbuf[out_rd];
		out_rd = NEXT_IDX(out_rd);
	} else {
		/* no more data to send for now, disable the interrupt */
		UCSR0B &= ~(1 << UDRIE0);
	}
}
