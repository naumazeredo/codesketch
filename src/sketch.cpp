#include "sketch.h"

#include "subprocess/subprocess.hpp"

#include <cstdio>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>

#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "externs.h"
#include "text.h"

namespace codesketch {

// Sketch variables
SketchState sketchState;
std::vector<SketchState> sketchStateStash;
int sketchSetup;

int mouseX, mouseY;
u8 mouseState;

int frameCount;

// Subprocess
fs::path sketchPath;
subprocess::Process sketchProcess;

// Window recreate
inline void recreateWindow() {
  auto winpos = window.getPosition();
  window.create(sf::VideoMode(windowWidth, windowHeight),
                windowTitle, windowStyle, windowSettings);
  window.setPosition(winpos);
}

inline void sketchSendData();
inline void sketchReceiveData();

inline void sketchInit() {
  frameCount = 0;

  sketchState.fillColor = sf::Color::Black;
  sketchState.strokeColor = sf::Color::Black;
  sketchState.strokeThickness = 0.0f;

  sketchStateStash.clear();

  window.clear();
}

bool sketchOpen(const std::string& name) {
  fs::path path { name };
  if (!fs::exists(path))
    return false;

  sketchPath = path;

  sketchProcess.open(sketchPath.string());

  return sketchIsRunning();
}

inline void sketchReadErrors() {
  std::string data;

  while (sketchProcess.read_stderr_line(data))
    printf("[sketch stderr] %s\n", data.c_str());
}

inline void restoreDefaults() {
  windowWidth     = defaultWindowWidth;
  windowHeight    = defaultWindowHeight;
  windowFramerate = defaultWindowFramerate;
  windowSettings.antialiasingLevel = 0;

  recreateWindow();
  window.setFramerateLimit(windowFramerate);
}

void sketchClose() {
  sketchProcess.kill();

  sketchPath.clear();

  restoreDefaults();
}

inline void sketchReceiveData() {
  std::string data;

  while (sketchProcess.read_stdout_line(data)) {
    std::istringstream cmd(data);
    int type;
    cmd >> type;

    /* Setup exclusive */
    if (type == COMMAND_FRAMERATE) {
      if (!sketchSetup) {
        printf("[sketch warning] Calling framerate from outside setup!\n");
        continue;
      }

      int r;
      cmd >> r;
      windowFramerate = r;
      window.setFramerateLimit(windowFramerate);
    }

    if (type == COMMAND_SMOOTH) {
      if (!sketchSetup) {
        printf("[sketch warning] Calling smooth from outside setup!\n");
        continue;
      }

      windowSettings.antialiasingLevel = 8;
      recreateWindow();
    }

    if (type == COMMAND_WINDOW) {
      if (!sketchSetup) {
        printf("[sketch warning] Calling window from outside setup!\n");
        continue;
      }

      int w, h;
      cmd >> w >> h;

      windowWidth  = w;
      windowHeight = h;
      recreateWindow();
    }
    /* -------------- */

    if (type == COMMAND_FRAMEEND) {
      // Verify incorrect frame ends
      // Push and pop imbalance
      if (sketchStateStash.size() > 0)
        printf("[sketch warning] Push/pop imbalance. For each push must exist a pop!\n");
      break;
    }

    if (type == COMMAND_DEBUG) {
      std::string text;
      getline(cmd, text);
      printf("[sketch debug]: %s\n", text.c_str());
    }

    if (type == COMMAND_BACKGROUND) {
      int r, g, b;
      cmd >> r >> g >> b;

      window.clear({ (u8)r, (u8)g, (u8)b });
    }

    if (type == COMMAND_POINT) {
      int x, y;
      cmd >> x >> y;

      float radius = std::max(1.0f, sketchState.strokeThickness) * 0.5f;
      sf::CircleShape point { radius };
      point.setFillColor(sketchState.strokeColor);
      point.setOrigin(radius, radius);
      point.setPosition(x, y);

      window.draw(point);
    }

    if (type == COMMAND_LINE) {
      int x0, y0, x1, y1;
      cmd >> x0 >> y0 >> x1 >> y1;

      float angle = std::atan2(y1-y0, x1-x0) * 180.0f / M_PI;
      float w = std::sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0));
      float h = std::max(1.0f, sketchState.strokeThickness);
      float radius = h * 0.5f;

      // Line
      sf::RectangleShape line { { w, h } };
      line.setFillColor(sketchState.strokeColor);
      line.setOrigin(0.0f, h / 2.0f);
      line.setPosition(x0, y0);
      line.rotate(angle);

      // Round ends
      sf::CircleShape end0 { radius };
      end0.setFillColor(sketchState.strokeColor);
      end0.setOrigin(radius, radius);
      end0.setPosition(x0, y0);

      sf::CircleShape end1 { radius };
      end1.setFillColor(sketchState.strokeColor);
      end1.setOrigin(radius, radius);
      end1.setPosition(x1, y1);

      window.draw(line);
      window.draw(end0);
      window.draw(end1);
    }

    if (type == COMMAND_RECT) {
      int x, y, w, h;
      cmd >> x >> y >> w >> h;

      sf::RectangleShape rect, strokes[4];
      sf::CircleShape corners[4];

      // Stroke
      for (int i = 0; i < 4; ++i)
        strokes[i].setFillColor(sketchState.strokeColor);

      // Top
      strokes[0].setSize({ (float)w, sketchState.strokeThickness });
      strokes[0].setPosition(x, y - sketchState.strokeThickness);

      // Bottom
      strokes[1].setSize({ (float)w, sketchState.strokeThickness });
      strokes[1].setPosition(x, y + h);

      // Left
      strokes[2].setSize({ sketchState.strokeThickness, (float)h });
      strokes[2].setPosition(x - sketchState.strokeThickness, y);

      // Right
      strokes[3].setSize({ sketchState.strokeThickness, (float)h });
      strokes[3].setPosition(x + w, y);

      // Round corners
      float radius = sketchState.strokeThickness;
      for (int i = 0; i < 4; ++i) {
        corners[i].setRadius(radius);
        corners[i].setFillColor(sketchState.strokeColor);
        corners[i].setOrigin(radius, radius);
        corners[i].setPosition(x + (i % 2) * w, y + (i / 2) * h);
      }

      // Fill
      rect.setSize({ (float)w, (float)h });
      rect.setFillColor(sketchState.fillColor);
      rect.setPosition(x, y);

      for (int i = 0; i < 4; ++i) {
        window.draw(strokes[i]);
        window.draw(corners[i]);
      }
      window.draw(rect);
    }

    if (type == COMMAND_CIRCLE) {
      int x, y, r;
      cmd >> x >> y >> r;

      float radius = r;
      sf::CircleShape circle { radius };
      circle.setFillColor(sketchState.fillColor);
      circle.setOrigin(radius, radius);
      circle.setPosition(x, y);

      circle.setOutlineThickness(sketchState.strokeThickness);
      circle.setOutlineColor(sketchState.strokeColor);

      window.draw(circle);
    }

    if (type == COMMAND_FILLCOLOR) {
      int r, g, b;
      cmd >> r >> g >> b;

      sketchState.fillColor = { (u8)r, (u8)g, (u8)b };
    }

    if (type == COMMAND_STROKECOLOR) {
      int r, g, b, a;
      cmd >> r >> g >> b >> a;

      sketchState.strokeColor = { (u8)r, (u8)g, (u8)b, (u8)a };
    }

    if (type == COMMAND_STROKETHICKNESS) {
      int t;
      cmd >> t;

      sketchState.strokeThickness = t;
    }


    if (type == COMMAND_TEXT) {
      int x, y;
      std::string text;
      cmd >> x >> y;
      getline(cmd, text);

      textRender(text, x, y);
    }

    if (type == COMMAND_TEXTSIZE) {
      int s;
      cmd >> s;

      textSetSize(s);
    }

    if (type == COMMAND_CAMERA) {
      int x, y;
      cmd >> x >> y;

      auto view = window.getView();
      view.setCenter(x + windowWidth / 2.0f, y + windowHeight / 2.0f);
      window.setView(view);
    }

    if (type == COMMAND_PUSH) {
      sketchStateStash.push_back(sketchState);
    }

    if (type == COMMAND_POP) {
      if (sketchStateStash.size() > 0) {
        sketchState = sketchStateStash.back();
        sketchStateStash.pop_back();
      } else {
        printf("[sketch warning] Trying to pop empty stash!\n");
      }
    }
  }

  sketchReadErrors();
}

inline void sketchSendData() {
  // Keyboard
  char keysstr[keysSize+1] = {};
  if (window.hasFocus()) {
    for (unsigned i = 0; i < keysSize; ++i)
      keysstr[i] = '0'+sf::Keyboard::isKeyPressed(keys[i]);
  } else {
    for (unsigned i = 0; i < keysSize; ++i)
      keysstr[i] = '0';
  }

  // Mouse
  auto mouse = sf::Mouse::getPosition(window);
  mouseX = mouse.x;
  mouseY = mouse.y;
  mouseState = 0;
  if (window.hasFocus()) {
    for (int i = sf::Mouse::Left; i < sf::Mouse::ButtonCount; ++i)
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button(i)))
        mouseState |= (1<<i);
  }

  char data[1024];
  sprintf(data, "%d %d %d %d %d %d %s\n",
          frameCount, windowWidth, windowHeight, mouseX, mouseY, mouseState, keysstr);

  sketchProcess.write(data, strlen(data));

  frameCount++;
}

bool sketchIsRunning() {
  return sketchProcess.is_active();
}

void sketchRun() {
  sketchSendData();
  sketchReceiveData();
}

}
