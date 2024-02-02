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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "midi.h"
#include "keyb.h"

static int parse_args(int argc, char **argv);

static int bottom[] = {
	0, 2, 4, 5, 7, 9, 11,  12, 14, 16
};
static int top[] = {
	-1, 1, 3, -1, 6, 8, 10, -1, 13, 15
};

static int port = 0x330;
static int chan = 0;
static int octave = 4;
static int octave_offs = 60;
static int prog = 0;
static int vel = 127;

static int note_state[128];

static const char *helptext =
	"Controls:\n"
	" Play using the bottom two rows of the keyboard. Esc to exit.\n"
	" change MIDI channel: 1-8    change instrument: (/)\n"
	" shift octave: [/]           change velocity: +/-\n";


int main(int argc, char **argv)
{
	int i, note, c;
	char *env, *ptr, *endp;

	if((env = getenv("BLASTER")) && ((ptr = strstr(env, "P:")) ||
				(ptr = strstr(env, "p:")))) {
		port = strtol(ptr + 2, &endp, 16);
		if(endp == ptr + 2) {
			fprintf(stderr, "invalid MPU port specified in BLASTER environment variable, ignoring\n");
			port = 0x330;
		}
	}

	if(parse_args(argc, argv) == -1) {
		return 1;
	}

	kb_init();

	printf("Initializing MIDI interface at port %x\n", port);
	if(midi_init(port) == -1) {
		fprintf(stderr, "failed to initialize MPU port\n");
		return 1;
	}
	midi_chprog(chan, prog);

	fputs(helptext, stdout);

	for(;;) {
		struct kb_event ev;
		c = kb_event(&ev);
		if(c == 27) {
			break;
		}

		if(ev.code >= 44 && ev.code <= 53) {
			note = bottom[ev.code - 44] + octave_offs;
			if(note_state[note] != ev.press) {
				midi_note(chan, note, ev.press ? vel : 0);
			}
			note_state[note] = ev.press;

		} else if(ev.code >= 30 && ev.code <= 40) {
			if((note = top[ev.code - 30]) != -1) {
				note += octave_offs;
				if(note_state[note] != ev.press) {
					midi_note(chan, note, ev.press ? vel : 0);
				}
				note_state[note] = ev.press;
			}
		} else if(c >= '1' && c <= '8') {
			chan = c - '1';
		} else {
			switch(c) {
			case '[':
				if(octave > 1) {
					octave_offs = --octave * 12 + 12;
					printf("octave: %d (midi offset: %d)\n", octave, octave_offs);
				}
				break;

			case ']':
				if(octave < 7) {
					octave_offs = ++octave * 12 + 12;
					printf("octave: %d (midi offset: %d)\n", octave, octave_offs);
				}
				break;

			case '9':
				prog = (prog - 1) & 0x7f;
				printf("[%d] instrument: %d\n", chan, prog);
				midi_chprog(chan, prog);
				break;

			case '0':
				prog = (prog + 1) & 0x7f;
				printf("[%d] instrument: %d\n", chan, prog);
				midi_chprog(chan, prog);
				break;

			case '-':
				if(vel > 0) {
					vel--;
					printf("velocity: %d\n", vel);
				}
				break;

			case '=':
				if(vel < 127) {
					vel++;
					printf("velocity: %d\n", vel);
				}
				break;
			}
		}
	}

	midi_shutdown();

	kb_shutdown();
	return 0;
}

static const char *usage_fmt = "Usage: %s [options]\n"
	"Options:\n"
	"  -p <port>: set MIDI base I/O port\n"
	"  -c <channel>: set MIDI channel\n"
	"  -i <instrument>: MIDI instrument\n"
	"  -o <octave>: select octave (1-7)\n"
	"  -h: print usage information and exit\n"
	"\n";

static int parse_args(int argc, char **argv)
{
	int i;
	char *endp;

	for(i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			if(argv[i][2]) {
				fprintf(stderr, "invalid option: %s\n", argv[i]);
				printf(usage_fmt, argv[0]);
				return -1;
			}
			switch(argv[i][1]) {
			case 'p':
				if(!argv[++i] || ((port = strtol(argv[i], &endp, 16)), endp == argv[i])) {
					fprintf(stderr, "-p should be followed by the port number in hex\n");
					return -1;
				}
				break;

			case 'c':
				if(!argv[++i] || !(chan = atoi(argv[i]))) {
					fprintf(stderr, "-c should be followed by the channel number\n");
					return -1;
				}
				break;

			case 'i':
				if(!argv[++i] || !(prog = atoi(argv[i]))) {
					fprintf(stderr, "-i should be followed by a valid channel number\n");
					return -1;
				}
				break;

			case 'o':
				if(!argv[++i] || !(octave = atoi(argv[i]))) {
					fprintf(stderr, "-o should be followed by the octane number.\n");
					return -1;
				}

			case 'h':
				printf(usage_fmt, argv[0]);
				exit(0);

			default:
				fprintf(stderr, "invalid option: %s\n", argv[i]);
				printf(usage_fmt, argv[0]);
				return -1;
			}
		}
		fprintf(stderr, "unexpected argument: %s\n", argv[i]);
		printf(usage_fmt, argv[0]);
		return -1;
	}
	return 0;
}
