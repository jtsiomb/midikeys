/*
DOS interrupt-based keyboard driver.
Copyright (C) 2013-2023  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License  for more details.

You should have received a copy of the GNU General Public License
along with the program. If not, see <http://www.gnu.org/licenses/>
*/
#ifndef KEYB_H_
#define KEYB_H_

/* special keys */
enum {
	KB_BACKSP = 8,
	KB_ESC = 27,
	KB_DEL = 127,

	KB_NUM_0, KB_NUM_1, KB_NUM_2, KB_NUM_3, KB_NUM_4,
	KB_NUM_5, KB_NUM_6, KB_NUM_7, KB_NUM_8, KB_NUM_9,
	KB_NUM_DOT, KB_NUM_DIV, KB_NUM_MUL, KB_NUM_MINUS, KB_NUM_PLUS, KB_NUM_ENTER, KB_NUM_EQUALS,
	KB_UP, KB_DOWN, KB_RIGHT, KB_LEFT,
	KB_INSERT, KB_HOME, KB_END, KB_PGUP, KB_PGDN,
	KB_F1, KB_F2, KB_F3, KB_F4, KB_F5, KB_F6,
	KB_F7, KB_F8, KB_F9, KB_F10, KB_F11, KB_F12,
	KB_F13, KB_F14, KB_F15,
	KB_NUMLK, KB_CAPSLK, KB_SCRLK,
	KB_RSHIFT, KB_LSHIFT, KB_RCTRL, KB_LCTRL, KB_RALT, KB_LALT,
	KB_RMETA, KB_LMETA, KB_LSUPER, KB_RSUPER, KB_MODE, KB_COMPOSE,
	KB_HELP, KB_PRINT, KB_SYSRQ, KB_BREAK
};

#define KB_ANY		(-1)
#define KB_ALT		(-2)
#define KB_CTRL		(-3)
#define KB_SHIFT	(-4)

struct kb_event {
	int key;
	int code;
	int press;
};

extern unsigned char keystate[256];

#ifdef __cplusplus
extern "C" {
#endif

int kb_init(void);
void kb_shutdown(void);

/* Returns the ASCII key event with bit 8 set of release, clear for press
 * kb_event filled if the pointer is non-null
 */
int kb_event(struct kb_event *ev);
int kb_pending(void);

#ifdef __cplusplus
}
#endif

#endif	/* KEYB_H_ */
