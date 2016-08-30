/* Using C++14 to access system_clock */

#include "sketch.h"
#include <chrono>
using namespace std::chrono;

system_clock::time_point past;
int fps, frames;

void setup() {
  framerate(20);
  textSize(24);
  fill(255, 255, 255);

  fps = 0;
  frames = 0;

  past = system_clock::now();
}

void draw() {
  system_clock::time_point now = system_clock::now();
  if (now >= past + 1s) {
    fps = frames;
    frames = 0;
    past = now;
  } else {
    frames++;
  }

  background(0, 0, 0);
  text(20, 20, "Frame: %d", frames);
  text(20, 40, "FPS  : %d", fps);
}
