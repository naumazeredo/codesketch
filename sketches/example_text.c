#include "sketch.h"

#include <math.h>

float frequency = 0.03f;

void setup() {
  textSize(20);
}

void draw() {
  background(0, 0, 0);

  int r = sin(frequency * frameCount + 0) * 127 + 128;
  int g = sin(frequency * frameCount + 2) * 127 + 128;
  int b = sin(frequency * frameCount + 4) * 127 + 128;

  fill(r, g, b);

  text("Code Sketch with text!", 20, frameCount % (windowHeight+20) - 20);
}
