#include "sketch.h"

void setup() {
  window(200, 200);
  fill(255, 255, 255);
}

void draw() {
  background(0, 0, 0);
  text(20, 20, "Window (%d, %d)", windowWidth, windowHeight);
}
