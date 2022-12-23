#ifndef MIDI_H_
#define MIDI_H_

int midi_init(int port);
void midi_shutdown(void);

void midi_note(int chan, int note, int vel);
void midi_chprog(int chan, int prog);
void midi_alloff(void);

int midi_send_cmd(int cmd);
int midi_send_data(int data);

#endif	/* MIDI_H_ */
