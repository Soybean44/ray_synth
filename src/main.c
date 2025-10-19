#include "common.h"
#define NUM_TABLES 2
const short* tables[NUM_TABLES] = {sineTable, sawTable};

void updateBuffer() {
  for (size_t i = 0; i < BUF_SIZE; i++) {
    // Linearly interpolate between the two nearest samples. 
    size_t idx_0 = (size_t)osc.idx;
    size_t idx_1 = ((size_t)(osc.idx + 1)) % TABLE_SIZE;
    short v0 = osc.table[idx_0]; 
    short v1 = osc.table[idx_1];
    float frac = osc.idx - (float)idx_0;
    if ((osc.idx += osc.delta) > TABLE_SIZE) 
      osc.idx -= (float)TABLE_SIZE;
    buffer[i] = (v0 + frac*(v1-v0));
  }
}

void audioCallback(void *bufferData, unsigned int frames) {
  short *d = (short *)bufferData;
  for (size_t i = 0; i < frames; i++) {
    d[i] = buffer[bufIdx];
    if (++bufIdx >= BUF_SIZE) {
      bufIdx = 0;
      updateBuffer();
    }
  }
}

int main(void) {
  InitWindow(800, 600, "ray-dsp");
  SetTargetFPS(60);

  // generate first buffer
  size_t currTable = 0;
  osc.table = tables[currTable];
  updateBuffer();

  // Initialize audio
  InitAudioDevice();
  AudioStream stream = LoadAudioStream(SAMPLE_RATE, 16, 1);
  SetAudioStreamBufferSizeDefault(BUF_SIZE);
  SetAudioStreamVolume(stream, 0.75);
  SetAudioStreamCallback(stream, audioCallback);
  PlayAudioStream(stream);

  Vector2 mousePosition = {0};

  while (!WindowShouldClose()) {
    mousePosition = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      currTable = (currTable + 1) % NUM_TABLES;
      osc.table = tables[currTable];
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      float fp = (float)(600-mousePosition.y);
      osc.freq = 40.0f + (float)(fp);
      osc.delta = osc.freq * TABLE_SIZE / SAMPLE_RATE;

    }
    BeginDrawing();
    ClearBackground(SKYBLUE);
    EndDrawing();
  }

  UnloadAudioStream(stream);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
