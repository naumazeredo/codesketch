#include "sketch.h"

#include <stdio.h>

int r = 0, g = 0, b = 0;

void setup() {
  textSize(20);
  fill(255, 255, 255);
}

void draw() {
  background(r, g, b);
  text("Red:   press Q or A", 20, 20);
  text("Green: press W or S", 20, 40);
  text("Blue:  press E or D", 20, 60);

  char buffer[256];
  sprintf(buffer, "Color: #%02X%02X%02X", r, g, b);
  text(buffer, 20, 80);

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
