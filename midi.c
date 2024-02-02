/*
midikeys - a simple MIDI keyboard for DOS PCs
Copyright (C) 2022-2024 John Tsiombikas <nuclear@mutantstargoat.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <conio.h>
#include "midi.h"

static int iobase = 0x330;

#define MPU_PORT_DATA	iobase
#define MPU_PORT_STAT	(iobase | 1)
#define MPU_PORT_CMD	(iobase | 1)

#define MPU_STAT_ORDY	0x40
#define MPU_STAT_IRDY	0x80

#define MPU_ACK				0xfe
#define MPU_CMD_RESET		0xff
#define MPU_CMD_UARTMODE	0x3f

#define MIDI_CMD_NOTEON		0x90
#define MIDI_CMD_NOTEOFF	0x80
#define MIDI_CMD_CHANMSG	0xb0
#define MIDI_CMD_CHPROG		0xc0

#define MIDI_CHANMSG_NOTESOFF	0x7b

int midi_init(int port)
{
	iobase = port;

	if(midi_send_cmd(MPU_CMD_RESET) == -1) {
		return -1;
	}
	if(midi_send_cmd(MPU_CMD_UARTMODE) == -1) {
		return -1;
	}
	midi_alloff();
	return 0;
}

void midi_shutdown(void)
{
	midi_alloff();
	midi_send_cmd(MPU_CMD_RESET);
}

void midi_note(int chan, int note, int vel)
{
	if(vel > 0) {
		midi_send_data(MIDI_CMD_NOTEON | chan);
	} else {
		midi_send_data(MIDI_CMD_NOTEOFF | chan);
	}
	midi_send_data(note);
	midi_send_data(vel);
}

void midi_chprog(int chan, int prog)
{
	midi_send_data(MIDI_CMD_CHPROG | chan);
	midi_send_data(prog);
}

void midi_alloff(void)
{
	midi_send_data(MIDI_CMD_CHANMSG);
	midi_send_data(MIDI_CHANMSG_NOTESOFF);
}

static int wait_ordy(void)
{
	int i;
	for(i=0; i<1024; i++) {
		if((inp(MPU_PORT_STAT) & MPU_STAT_ORDY) == 0) {
			return 0;
		}
	}
	return -1;
}

int midi_send_cmd(int cmd)
{
	if(wait_ordy() == -1) {
		return -1;
	}
	outp(MPU_PORT_CMD, cmd);
	return 0;
}

int midi_send_data(int data)
{
	if(wait_ordy() == -1) {
		return -1;
	}
	outp(MPU_PORT_DATA, data);
	return 0;
}
