#include "sketch.h"

// setup is executed only once at the beginning
void setup() {
}

// draw is executed around 60 times per second
void draw() {
  background(127, 127, 127);

  fill(255, 255, 255);
  circle(100, 100, 20);

  push();
  fill(255, 0, 0);
  stroke(0, 0, 0);
  strokeThickness(2);
  circle(140, 100, 20);
  pop();

  circle(180, 100, 20);
}
