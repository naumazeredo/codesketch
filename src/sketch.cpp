#include "sketch.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "externs.h"
#include "text.h"

namespace codesketch {

// Sketch variables
sf::Color fillColor = sf::Color::Black;
sf::Color strokeColor = sf::Color::Black;
float strokeThickness = 0.0f;

// Subprocess
pid_t sketchpid = 0;
int sketchin[2], sketchout[2], sketcherr[2];
int sketchStatus = 0;

inline void sketchInit() {
  frameCount = 0;
  fillColor = sf::Color::Black;
  strokeColor = sf::Color::Black;
  strokeThickness = 0.0f;

  window.clear();
}

bool sketchOpen(const std::string& name) {
  // TODO(naum): treat errors
  if (pipe(sketchin) == -1) return false;
  if (pipe(sketchout) == -1) return false;
  if (pipe(sketcherr) == -1) return false;

  sketchpid = fork();

  if (sketchpid == 0) {
    // Child process (sketch);

    if (dup2(sketchin[READ],   STDIN_FILENO)  == -1) return false;
    if (dup2(sketchout[WRITE], STDOUT_FILENO) == -1) return false;
    if (dup2(sketcherr[WRITE], STDERR_FILENO) == -1) return false;

    close(sketchin[READ]);
    close(sketchin[WRITE]);
    close(sketchout[READ]);
    close(sketchout[WRITE]);
    close(sketcherr[READ]);
    close(sketcherr[WRITE]);

    if (execl(name.c_str(), name.c_str(), nullptr) == -1) {
      // Exec fail: kill child!
      kill(getpid(), SIGKILL);
    }
  } else if (sketchpid > 0) {
    // Parent process

    close(sketchin[READ]);
    close(sketchout[WRITE]);
    close(sketcherr[WRITE]);

    //fcntl(sketchout[READ], F_SETFL, fcntl(sketchout[READ], F_GETFL, 0) | O_NONBLOCK);
    //fcntl(sketcherr[READ], F_SETFL, fcntl(sketcherr[READ], F_GETFL, 0) | O_NONBLOCK);

    //fcntl(sketchout[READ], F_SETPIPE_SZ, 1048576);
  } else {
    // Error

    close(sketchin[READ]);
    close(sketchin[WRITE]);
    close(sketchout[READ]);
    close(sketchout[WRITE]);
    close(sketcherr[READ]);
    close(sketcherr[WRITE]);

    return false;
  }

  sketchInit();

  return sketchIsRunning();
}

void sketchClose() {
  if (!sketchIsRunning()) return;

  kill(sketchpid, SIGKILL);
  close(sketchin[WRITE]);
  close(sketchout[READ]);
  close(sketcherr[READ]);
  sketchpid = 0;
}

bool sketchIsRunning() {
  sketchStatus = 0;
  if (sketchpid == 0 or waitpid(sketchpid, &sketchStatus, WNOHANG) != 0) {
    sketchpid = 0;
    return false;
  }

  return true;
}

inline void sketchReceiveData() {
  std::string data;

  while (sketchIsRunning()) {
    data = "";

    char c;
    int bytesread;
    while ((bytesread = read(sketchout[READ], &c, 1)) > 0 and c != '\n')
      data += c;

    // Error reading pipe or end of file
    if (bytesread <= 0)
      break;

    std::istringstream cmd(data);
    int type;
    cmd >> type;

    if (type == COMMAND_FRAMEEND)
      break;

    if (type == COMMAND_BACKGROUND) {
      int r, g, b;
      cmd >> r >> g >> b;

      window.clear({ (u8)r, (u8)g, (u8)b });
    }

    if (type == COMMAND_POINT) {
      int x, y;
      cmd >> x >> y;

      float radius = std::max(1.0f, strokeThickness) * 0.5f;
      sf::CircleShape point { radius };
      point.setFillColor(strokeColor);
      point.setOrigin(radius, radius);
      point.setPosition(x, y);

      window.draw(point);
    }

    if (type == COMMAND_LINE) {
      int x0, y0, x1, y1;
      cmd >> x0 >> y0 >> x1 >> y1;

      float angle = std::atan2(y1-y0, x1-x0) * 180.0f / M_PI;
      float w = std::sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0));
      float h = std::max(1.0f, strokeThickness);
      float radius = h * 0.5f;

      // Line
      sf::RectangleShape line { { w, h } };
      line.setFillColor(strokeColor);
      line.setOrigin(0.0f, h / 2.0f);
      line.setPosition(x0, y0);
      line.rotate(angle);

      // Round ends
      sf::CircleShape end0 { radius };
      end0.setFillColor(strokeColor);
      end0.setOrigin(radius, radius);
      end0.setPosition(x0, y0);

      sf::CircleShape end1 { radius };
      end1.setFillColor(strokeColor);
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
        strokes[i].setFillColor(strokeColor);

      // Top
      strokes[0].setSize({ (float)w, strokeThickness });
      strokes[0].setPosition(x, y - strokeThickness);

      // Bottom
      strokes[1].setSize({ (float)w, strokeThickness });
      strokes[1].setPosition(x, y + h);

      // Left
      strokes[2].setSize({ strokeThickness, (float)h });
      strokes[2].setPosition(x - strokeThickness, y);

      // Right
      strokes[3].setSize({ strokeThickness, (float)h });
      strokes[3].setPosition(x + w, y);

      // Round corners
      float radius = strokeThickness;
      for (int i = 0; i < 4; ++i) {
        corners[i].setRadius(radius);
        corners[i].setFillColor(strokeColor);
        corners[i].setOrigin(radius, radius);
        corners[i].setPosition(x + (i % 2) * w, y + (i / 2) * h);
      }

      // Fill
      rect.setSize({ (float)w, (float)h });
      rect.setFillColor(fillColor);
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
      circle.setFillColor(fillColor);
      circle.setOrigin(radius, radius);
      circle.setPosition(x, y);

      circle.setOutlineThickness(strokeThickness);
      circle.setOutlineColor(strokeColor);

      window.draw(circle);
    }

    if (type == COMMAND_FILLCOLOR) {
      int r, g, b;
      cmd >> r >> g >> b;

      fillColor = { (u8)r, (u8)g, (u8)b };
    }

    if (type == COMMAND_STROKECOLOR) {
      int r, g, b, a;
      cmd >> r >> g >> b >> a;

      strokeColor = { (u8)r, (u8)g, (u8)b, (u8)a };
    }

    if (type == COMMAND_STROKETHICKNESS) {
      int t;
      cmd >> t;

      strokeThickness = t;
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

    if (type == COMMAND_TEXTCOLOR) {
      int r, g, b, a;
      cmd >> r >> g >> b >> a;

      textSetColor(r, g, b, a);
    }
  }
}

inline void sketchSendData() {
  char keysstr[keysSize+1] = {};
  for (unsigned i = 0; i < keysSize; ++i)
    keysstr[i] = '0'+sf::Keyboard::isKeyPressed(keys[i]);

  char data[1024];
  sprintf(data, "%d %d %d %d %d %d %s\n",
          frameCount, windowWidth, windowHeight, mouseX, mouseY, mouseState, keysstr);

  write(sketchin[WRITE], data, strlen(data));
  frameCount++;
}

void sketchRun() {
  sketchSendData();
  sketchReceiveData();
}

}
