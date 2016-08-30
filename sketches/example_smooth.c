#include "sketch.h"

void setup() {
  smooth();
  fill(0, 0, 0);
}

void draw() {
  background(255, 255, 255);

  fill(0, 0, 0);
  circle(windowWidth / 2, windowHeight / 2, 100);

  stroke(255, 0, 0);
  line(0, 0, windowWidth, windowHeight);

  stroke(0, 0, 255);
  line(0, windowHeight, windowWidth, 0);
}
