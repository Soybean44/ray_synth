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
static short audioBuffer[BUF_SIZE] = {0};
static size_t bufIdx = 0;

typedef struct {
  float freq;
  float amplitude;
  const short *table;
  float idx;
} Oscilator;

