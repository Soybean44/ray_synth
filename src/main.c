#include "common.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define WIDTH 1600
#define HEIGHT 900
#define FPS 120
#define NUM_TABLES 2
const short *tables[NUM_TABLES] = {sineTable, sawTable};
size_t currTable = 0;

Oscilator osc = {
    .freq = 440.0f,
    .amplitude = 1.0,
    .table = sineTable,
    .idx = 0.0f,
};


void updateBuffer(short *buffer, size_t size, Oscilator *osc) {
  for (size_t i = 0; i < size; i++) {
    // Linearly interpolate between the two nearest samples.
    size_t idx_0 = (size_t)osc->idx;
    size_t idx_1 = (size_t)osc->idx + 1;
    short v0 = osc->table[idx_0];
    short v1 = osc->table[idx_1];
    float frac = osc->idx - (float)idx_0;

    osc->idx += osc->freq * (float)TABLE_SIZE / (float)SAMPLE_RATE;
    if (osc->idx >= TABLE_SIZE)
      osc->idx -= (float)TABLE_SIZE;

    buffer[i] = osc->amplitude * (v0 + frac * (v1 - v0));
  }
}

void audioCallback(void *bufferData, unsigned int frames) {
  short *d = (short *)bufferData;
  for (size_t i = 0; i < frames; i++) {
    d[i] = audioBuffer[bufIdx];
    if (++bufIdx >= BUF_SIZE) {
      bufIdx = 0;
      updateBuffer(audioBuffer, BUF_SIZE, &osc);
    }
  }
}

int main(void) {
  InitWindow(WIDTH, HEIGHT, "ray-dsp");
  SetTargetFPS(FPS);

  midi_init();
  // generate first buffers
  short visBuffer[WIDTH];
  Oscilator visOsc = osc;
  updateBuffer(audioBuffer, BUF_SIZE, &osc);
  updateBuffer(visBuffer, WIDTH, &visOsc);

  // Initialize audio
  InitAudioDevice();
  AudioStream stream = LoadAudioStream(SAMPLE_RATE, 16, 1);
  SetAudioStreamVolume(stream, 0.35);
  SetAudioStreamCallback(stream, audioCallback);

  PlayAudioStream(stream);

  MidiEvent prev_event = {-1};
  // Vector2 mousePosition = {0};
  while (!WindowShouldClose()) {
    // mousePosition = GetMousePosition();

    MidiEvent event = get_midi_event();
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      currTable = (currTable + 1) % NUM_TABLES;
      osc.table = tables[currTable];
      visOsc.table = tables[currTable];
      updateBuffer(visBuffer, WIDTH, &visOsc);
    }
    if (event.on == 1) {
      osc.freq = event.freq;
      osc.amplitude = (float)event.vel / 127.0f;
      visOsc.freq = event.freq;
      visOsc.amplitude = (float)event.vel / 127.0f;
      updateBuffer(visBuffer, WIDTH, &visOsc);
      prev_event = event;
    } else if (event.on == 0 && event.note == prev_event.note) {
      osc.amplitude = 0.0f;
      visOsc.amplitude = 0.0f;
      updateBuffer(visBuffer, WIDTH, &visOsc);
    }

    BeginDrawing();
    ClearBackground(BLACK);
    for (size_t x = 0; x < WIDTH; x++) {
      DrawPixel(x, HEIGHT / 2 + HEIGHT * visBuffer[x/2] / 64000, RED);
    }
    EndDrawing();
  }

  UnloadAudioStream(stream);
  CloseAudioDevice();
  midi_terminate();
  CloseWindow();
  return 0;
}
