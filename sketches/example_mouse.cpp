#include "sketch.h"

#include <vector>
using namespace std;

vector<int> x, y;

void setup() {
  x.push_back(windowWidth/2);
  y.push_back(windowHeight/2);
}

void draw() {
  background(255, 255, 255);

  for (int i = 1; i < x.size(); ++i)
    line(x[i-1], y[i-1], x[i], y[i]);

  line(x.back(), y.back(), mouseX, mouseY);

  if (mousePressed(MOUSE_LEFT)) {
    x.push_back(mouseX);
    y.push_back(mouseY);
  }
}
