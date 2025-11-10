#pragma once

typedef struct {
    int on;
    int note;
    int vel;
    float freq;
} MidiEvent;

void midi_init(void);
MidiEvent get_midi_event(void);
void midi_terminate(void);
