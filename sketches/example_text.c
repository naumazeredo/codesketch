#include "sketch.h"

#include <math.h>

int y = 20;
float frequency = 0.03f;

void setup() {
  textSize(20);
}

void draw() {
  background(0, 0, 0);

  int r = sin(frequency * y + 0) * 127 + 128;
  int g = sin(frequency * y + 2) * 127 + 128;
  int b = sin(frequency * y + 4) * 127 + 128;

  textColor(r, g, b);

  text("Code Sketch with text!", 20, y);

  y++;
  y = y % windowHeight;
}
