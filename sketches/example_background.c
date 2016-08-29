#include "sketch.h"

#include <stdio.h>

int r = 0, g = 0, b = 0;

void setup() {
  textSize(24);
  fill(255, 255, 255);
}

void draw() {
  background(r, g, b);
  text(20, 20, "Red:   press Q or A");
  text(20, 40, "Green: press W or S");
  text(20, 60, "Blue:  press E or D");
  text(20, 80, "Color: #%02X%02X%02X", r, g, b);

  if (keyDown(KEY_Q)) r++;
  if (keyDown(KEY_A)) r--;
  if (keyDown(KEY_W)) g++;
  if (keyDown(KEY_S)) g--;
  if (keyDown(KEY_E)) b++;
  if (keyDown(KEY_D)) b--;

  if (r > 255) r = 255;
  if (r < 0) r = 0;
  if (g > 255) g = 255;
  if (g < 0) g = 0;
  if (b > 255) b = 255;
  if (b < 0) b = 0;
}
