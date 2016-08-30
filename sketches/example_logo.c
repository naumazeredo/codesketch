#include "sketch.h"
#include <math.h>

const int NUM_LINES = 20;

int x, y;
int radius = 10;
float k = 0.02f;

// setup is executed only once at the beginning
void setup() {
  strokeThickness(1);
  textSize(24);
}

// draw is executed around 60 times per second
void draw() {
  background(127, 140, 147);

  fill(0, 0, 0);
  for (int i = 0; i < NUM_LINES; ++i) {
    line(i * windowWidth / NUM_LINES, 0,
         windowWidth, i * windowHeight / NUM_LINES);

    line(0, i * windowHeight / NUM_LINES,
         i * windowWidth / NUM_LINES, windowHeight);
  }

  text(20, 20, "Welcome to Code Sketch!");

  float s = (1 + sin(frameCount * k))/2;
  x = windowWidth * s;
  y = windowHeight * s;

  fill(255, 255, 255);
  circle(x, y, radius);

}
