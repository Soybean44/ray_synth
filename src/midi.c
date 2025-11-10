#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <portmidi.h>
#include <raylib.h>
#include "raygui.h"
#include "midi.h"

PmDeviceID input;
PortMidiStream *midiStream;

void midi_init(void) {
  Pm_Initialize();
  input = Pm_CreateVirtualInput("ray_synth Input", "ALSA", NULL);
  Pm_OpenInput(&midiStream, input, NULL, 512, NULL, NULL);
  printf("MIDI initialized\n");
}

MidiEvent get_midi_event(void) {
  if (Pm_Poll(midiStream) == 0) {
    return (MidiEvent){-1, 0, 0}; // No events
  }
  PmEvent event;
  MidiEvent result = {0};
  int numEvents = Pm_Read(midiStream, &event, 1);
  if (numEvents > 0) {
    unsigned char status = Pm_MessageStatus(event.message);
    unsigned char data1 = Pm_MessageData1(event.message);
    unsigned char data2 = Pm_MessageData2(event.message);

    if ((status & 0xF0) == 0x90 && data2 != 0) { // Note On
      int note = data1;
      int velocity = data2;
      float freq = 440.0f * powf(2.0f, (note - 69) / 12.0f);
      result.on = 1;
      result.note = note;
      result.vel = velocity;
      result.freq = freq;
      printf("Note On: %d Velocity: %d Frequency: %.2f Hz\n", note, velocity, freq);
    } else if ((status & 0xF0) == 0x80 || ((status & 0xF0) == 0x90 && data2 == 0)) { // Note Off
      int note = data1;
      result.on = 0;
      result.note = note;
      result.vel = 0;
      result.freq = 0.0f;
      printf("Note Off: %d\n", note);
    }
  }
  return result;
}

void midi_terminate(void) {
  Pm_Terminate();
  Pm_DeleteVirtualDevice(input);
  Pm_Close(midiStream);
  printf("MIDI terminated\n");
}
