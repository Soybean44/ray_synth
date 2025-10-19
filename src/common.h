#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gen.h"

#define SAMPLE_RATE 44100
#define BUF_SIZE 512
static short buffer[BUF_SIZE] = {0};
static size_t bufIdx = 0;

typedef struct {
  float freq;
  float amplitude;
  const short *table;
  float idx;
  float delta;
} Oscilator;

static Oscilator osc = {.freq = 440.0f,
                        .amplitude = 32000.0f,
                        .table = sineTable,
                        .idx = 0.0f,
                        // .delta = (freq*TABLE_SIZE)/SAMPLE_RATE
                        .delta = 440.0f * TABLE_SIZE / SAMPLE_RATE};
