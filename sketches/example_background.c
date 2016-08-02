#include "sketch.h"

int r = 0, g = 0, b = 0;

void setup() {
}

void draw() {
  background(r, g, b);

  if (keyPressed(KEY_Q)) r++;
  if (keyPressed(KEY_A)) r--;
  if (keyPressed(KEY_W)) g++;
  if (keyPressed(KEY_S)) g--;
  if (keyPressed(KEY_E)) b++;
  if (keyPressed(KEY_D)) b--;

  if (r > 255) r = 255;
  if (r < 0) r = 0;
  if (g > 255) g = 255;
  if (g < 0) g = 0;
  if (b > 255) b = 255;
  if (b < 0) b = 0;
}
