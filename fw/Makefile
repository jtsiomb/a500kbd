# --- Options -----------------------------------
#
# --- MCU part ---
mcu_gcc = atmega168p
mcu_dude = m168p
mcu_minipro = ATMEGA168PA
#mcu_gcc = atmega88p
#mcu_dude = m88p
#mcu_minipro = ATMEGA88PA
# --- Clock ---
# External crystal (14.7456 MHz):
#lfuse = e6
#ckfreq = 14745600
# 8MHz internal oscillator:
lfuse = e2
ckfreq = 8000000
# --- Programmer ---
# Lady ADA's USBtinyISP
prog=usbtiny
# USBasp clone
#prog=usbasp-clone
# -----------------------------------------------

src = $(wildcard src/*.c)
obj = $(src:.c=.o)
bin = a500kbd
hex = $(bin).hex
eep = $(bin).eep

CC = avr-gcc
OBJCOPY = avr-objcopy

CFLAGS = -Os -pedantic -Wall -mmcu=$(mcu_gcc) -DF_CPU=$(ckfreq)
LDFLAGS = -Wl,-Map,$(bin).map -mmcu=$(mcu_gcc) -lprintf_min

.PHONY: all
all: $(hex) $(eep)

$(bin): $(obj)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(hex): $(bin)
	$(OBJCOPY) -j .text -j .data -O ihex -R .eeprom $< $@

$(eep): $(bin)
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

.PHONY: fuses
fuses:
	avrdude -c $(prog) -p $(mcu_dude) -U lfuse:w:0x$(lfuse):m -U hfuse:w:0xdf:m -U efuse:w:0xf9:m

.PHONY: program
program: $(hex)
	avrdude -c $(prog) -p $(mcu_dude) -e -U flash:w:$(hex)

.PHONY: clean
clean:
	rm -f $(bin) $(obj) $(hex) $(eep) $(bin).map

.PHONY: burn
burn:
	minipro -p $(mcu_minipro) -s -w $(hex)

.PHONY: burnfuses
burnfuses:
	echo 'fuses_lo = 0x00$(lfuse)' >/tmp/fuses
	echo 'fuses_hi = 0x00df' >>/tmp/fuses
	echo 'fuses_ext = 0x00f9' >>/tmp/fuses
	echo 'lock_byte = 0x00ff' >>/tmp/fuses
	minipro -p $(mcu_minipro) -e -c config -w /tmp/fuses

