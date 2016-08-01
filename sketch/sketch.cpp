#include "sketch.h"

int x = 100, y = 100;

void setup() {
}

void draw() {
  background(200, 200, 200);

  for (int i = 0; i < 480; i+=9)
    line(i*4/3, 0, 640, i);

  rect(x, y, 50, 50);

  if (keyPressed(KEY_UP)) y--;
  if (keyPressed(KEY_DOWN)) y++;
  if (keyPressed(KEY_RIGHT)) x++;
  if (keyPressed(KEY_LEFT)) x--;
}
