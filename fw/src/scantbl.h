#ifndef SCANTBL_H_
#define SCANTBL_H_

/*
enum {
	KEY_ESC = 27,

	KEY_F1 = 256,
	KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8,
	KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_SYSRQ, KEY_BRK,
	KEY_SCRLK, KEY_CAPSLK, KEY_NUMLK,

	KEY_LSHIFT, KEY_RSHIFT,
	KEY_LCTRL, KEY_RCTRL,
	KEY_LALT, KEY_RALT,
	KEY_LWIN, KEY_RWIN, KEY_MENU,

	KEY_INS, KEY_DEL, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDOWN,
	KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,

	KEY_KP_0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4,
	KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9,
	KEY_KP_DIV, KEY_KP_MUL, KEY_KP_MINUS, KEY_KP_PLUS,
	KEY_KP_DOT, KEY_KP_ENTER,

	KEY_MM_WWW_SEARCH, KEY_MM_WWW_FAV, KEY_MM_WWW_REFRESH, KEY_MM_WWW_STOP,
	KEY_MM_WWW_FWD, KEY_MM_WWW_BACK, KEY_MM_WWW_HOME,
	KEY_MM_PREV, KEY_MM_NEXT, KEY_MM_PLAY, KEY_MM_STOP, KEY_MM_VOLUP, KEY_MM_VOLDOWN,
	KEY_MM_MUTE, KEY_MM_CALC, KEY_MM_MYCOMP, KEY_MM_EMAIL, KEY_MM_SELECT,
	KEY_APPS,
	KEY_ACPI_POWER, KEY_ACPI_SLEEP, KEY_ACPI_WAKEUP
};
*/

#define KEYMAP_NORMAL_SIZE	(sizeof keymap_normal / sizeof *keymap_normal)
#if 0
static int keymap_normal[] = {
	0, KEY_F9, 0, KEY_F5, KEY_F3, KEY_F1, KEY_F2, KEY_F12,	/* 00 - 07 */
	0, KEY_F10, KEY_F8, KEY_F6,	KEY_F4, '\t', '`', 0,		/* 08 - 0f */
	0, KEY_LALT, KEY_LSHIFT, 0, KEY_LCTRL, 'Q', '1', 0,		/* 10 - 17 */
	0, 0, 'Z', 'S', 'A', 'W', '2', 0,						/* 18 - 1f */
	0, 'C', 'X', 'D', 'E', '4', '3', 0,						/* 20 - 27 */
	0, ' ', 'V', 'F', 'T', 'R', '5', 0,						/* 28 - 2f */
	0, 'N', 'B', 'H', 'G', 'Y', '6', 0,						/* 30 - 37 */
	0, 0, 'M', 'J', 'U', '7', '8', 0,						/* 38 - 3f */
	0, ',', 'K', 'I', 'O', '0', '9', 0,						/* 40 - 47 */
	0, '.', '/', 'L', ';', 'P', '-', 0,						/* 48 - 4f */
	0, 0, '\'', 0, '[', '=', 0, 0,							/* 50 - 57 */
	KEY_CAPSLK, KEY_RSHIFT, '\n', ']', 0, '\\', 0, 0,		/* 58 - 5f */
	0, 0, 0, 0, 0, 0, '\b', 0,								/* 60 - 67 */
	0, KEY_KP_1, 0, KEY_KP_4, KEY_KP_7, 0, 0, 0,				/* 68 - 6f */
	KEY_KP_0, KEY_KP_DOT, KEY_KP_2, KEY_KP_5, KEY_KP_6, KEY_KP_8, KEY_ESC, KEY_NUMLK, /* 70 - 77 */
	KEY_F11, KEY_KP_PLUS, KEY_KP_3, KEY_KP_MINUS, KEY_KP_MUL, KEY_KP_9, KEY_SCRLK, 0,/* 78 - 7f */
	0, 0, 0, KEY_F7											/* 80 - 83 */
};
#endif

/* converts a normal (non-extended) PS/2 scancode to an amiga rawkey value
 * caps lock -> control
 * ctrl -> amiga
 * F12 -> help
 */
static unsigned char keymap_normal[] = {
	0xff, 0x58, 0xff, 0x54, 0x52, 0x50, 0x51, 0x5f,		/* 00 - 07: - f9 - f5 f3 f1 f2 f12 */
	0xff, 0x59, 0x57, 0x55, 0x53, 0x42, 0x00, 0xff,		/* 08 - 0f: - f10 f8 f6 f4 tab ` - */
	0xff, 0x64, 0x60, 0xff, 0x66, 0x10, 0x01, 0xff,		/* 10 - 17: - lalt lshift - lctrl(lamiga) Q 1 - */
	0xff, 0xff, 0x31, 0x21, 0x20, 0x11, 0x02, 0xff,		/* 18 - 1f: - - Z S A W 2 - */
	0xff, 0x33, 0x32, 0x22, 0x12, 0x04, 0x03, 0xff,		/* 20 - 27: - C X D E 4 3 - */
	0xff, 0x40, 0x34, 0x23, 0x14, 0x13, 0x05, 0xff,		/* 28 - 2f: - space V F T R 5 - */
	0xff, 0x36, 0x35, 0x25, 0x24, 0x15, 0x06, 0xff,		/* 30 - 37: - N B H G Y 6 - */
	0xff, 0xff, 0x37, 0x26, 0x16, 0x07, 0x08, 0xff,		/* 38 - 3f: - - M J U 7 8 - */
	0xff, 0x38, 0x27, 0x17, 0x18, 0x0a, 0x09, 0xff,		/* 40 - 47: - , K I O 0 9 - */
	0xff, 0x39, 0x3a, 0x28, 0x29, 0x19, 0x0b, 0xff,		/* 48 - 4f: - . / L ; P minus - */
	0xff, 0xff, 0x2a, 0xff, 0x1a, 0x0c, 0xff, 0xff,		/* 50 - 57: - - \ - [ = - - */
	0x63, 0x61, 0x44, 0x1b, 0xff, 0x0d, 0xff, 0xff,		/* 58 - 5f: caps rshift enter ] - \ - - */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x41, 0xff,		/* 60 - 67: - - - - - - backsp - */
	0xff, 0x1d, 0xff, 0x2d, 0x3d, 0xff, 0xff, 0xff,		/* 68 - 6f: - KP1 - KP4 KP7 - - - */
	0x0f, 0x3c, 0x1e, 0x2e, 0x2f, 0x3e, 0x45, 0x5a,		/* 70 - 77: KP0 KP. KP2 KP5 KP6 KP8 esc numlk */
	0xff, 0x5e, 0x1f, 0x4a, 0x5d, 0x3f, 0x62, 0xff,		/* 78 - 7f: f11 KP+ KP3 KP- KP* KP9 scrlk(capslock) - */
	0xff, 0xff, 0xff, 0x56								/* 80 - 83: - - - f7 */
};

#define KEYMAP_EXT_SIZE	(sizeof keymap_ext / sizeof *keymap_ext)
#if 0
static int keymap_ext[] = {
	0, 0, 0, 0, 0, 0, 0, 0,									/* 00 - 07 */
	0, 0, 0, 0, 0, 0, 0, 0,									/* 08 - 0f */
	KEY_MM_WWW_SEARCH, KEY_RALT, 0, 0, KEY_RCTRL, KEY_MM_PREV, 0, 0,	/* 10 - 17 */
	KEY_MM_WWW_FAV, 0, 0, 0, 0, 0, 0, KEY_LWIN,				/* 18 - 1f */
	KEY_MM_WWW_REFRESH, KEY_MM_VOLDOWN, 0, KEY_MM_MUTE, 0, 0, 0, KEY_RWIN,	/* 20 - 27 */
	KEY_MM_STOP, 0, 0, KEY_MM_CALC, 0, 0, 0, KEY_APPS,		/* 28 - 2f */
	KEY_MM_WWW_FWD, 0, KEY_MM_VOLUP, 0, KEY_MM_PLAY, 0, 0, KEY_ACPI_POWER,	/* 30 - 37 */
	KEY_MM_WWW_BACK, 0, KEY_MM_WWW_HOME, KEY_MM_STOP, 0, 0, 0, KEY_ACPI_SLEEP,	/* 38 - 3f */
	KEY_MM_MYCOMP, 0, 0, 0, 0, 0, 0, 0,						/* 40 - 47 */
	KEY_MM_EMAIL, 0, KEY_KP_DIV, 0, 0, KEY_MM_NEXT, 0, 0,	/* 48 - 4f */
	KEY_MM_SELECT, 0, 0, 0, 0, 0, 0, 0,						/* 50 - 57 */
	0, 0, KEY_KP_ENTER, 0, 0, 0, KEY_ACPI_WAKEUP, 0,		/* 58 - 5f */
	0, 0, 0, 0, 0, 0, 0, 0,									/* 60 - 67 */
	0, KEY_END, 0, KEY_LEFT, KEY_HOME, 0, 0, 0,				/* 68 - 6f */
	KEY_INS, KEY_DEL, KEY_DOWN, 0, KEY_RIGHT, KEY_UP, 0, 0,	/* 70 - 77 */
	0, 0, KEY_PGDOWN, 0, 0, KEY_PGUP, 0, 0,					/* 78 - 7f */
};
#endif

static unsigned char keymap_ext[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 00 - 07: - - - - - - - - */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 08 - 0f: - - - - - - - - */
	0xff, 0x65, 0xff, 0xff, 0x67, 0xff, 0xff, 0xff,		/* 10 - 17: - ralt - - rctrl(ramiga) - - - */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x66,		/* 18 - 1f: - - - - - - - lwin(lamiga) */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x67,		/* 20 - 27: - - - - - - - rwin(ramiga) */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 28 - 2f */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 30 - 37 */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 38 - 3f */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 40 - 47 */
	0xff, 0xff, 0x5c, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 48 - 4f: - - KP/ - - - - - */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 50 - 57 */
	0xff, 0xff, 0x43, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 58 - 5f: - - KP_enter - - - - - */
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,		/* 60 - 67 */
	0xff, 0x1d, 0xff, 0x4f, 0x3d, 0xff, 0xff, 0xff,		/* 68 - 6f: - end - left home - - - */
	0x0f, 0x46, 0x4d, 0xff, 0x4e, 0x4c, 0xff, 0xff,		/* 70 - 77: ins del down - right up - - */
	0xff, 0xff, 0x1f, 0xff, 0xff, 0x3f, 0xff, 0xff		/* 78 - 7f: - - pgdown - - pgup - - */
};

/* some useful keycodes */
#define AMIKEY_CTRL		0x63
#define AMIKEY_LAMI		0x66
#define AMIKEY_RAMI		0x67

#endif	/* SCANTBL_H_ */
