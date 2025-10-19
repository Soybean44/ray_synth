#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846f
static int tableSize = 512;

void genTable(char* tableName, FILE *fd, short (*nextSample)(float)) {
  float phase = 0, phase_inc = 1.0f / (tableSize); 
  fprintf(fd, "static const short %s[TABLE_SIZE+1] = {", tableName);
  for (size_t i = 0; i <= tableSize; i++) {
    fprintf(fd, "%hd", nextSample(phase));
    phase += phase_inc;
    if (i < tableSize)
      fprintf(fd, ", ");
  }
  fprintf(fd, "};\n\n");
}

short sineSample(float phase) {
  return (short)(32000.0f * sinf(2*PI * phase));
}
short sawSample(float phase) {
  return (short)(-32000.0f + phase * 64000.0f);
}

int main(void) {
  FILE *fd = fopen("./src/gen.h", "w");
  fprintf(fd, "#define TABLE_SIZE %d\n", tableSize);
  genTable("sineTable", fd, sineSample);
  genTable("sawTable", fd, sawSample);
}
