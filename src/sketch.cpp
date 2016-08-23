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

sf::Color fillColor = sf::Color::Black;
sf::Color strokeColor = sf::Color::Black;
float strokeThickness = 0.0f;

// Subprocess
pid_t sketchpid = 0;
int sketchin[2], sketchout[2], sketcherr[2];
int sketchstatus = 0;

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

    fcntl(sketchout[READ], F_SETFL, fcntl(sketchout[READ], F_GETFL, 0) | O_NONBLOCK);
    fcntl(sketcherr[READ], F_SETFL, fcntl(sketcherr[READ], F_GETFL, 0) | O_NONBLOCK);

    fcntl(sketchout[READ], F_SETPIPE_SZ, 1048576);
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

  frameCount = 0;

  usleep(1);
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
  sketchstatus = 0;
  if (sketchpid == 0 or waitpid(sketchpid, &sketchstatus, WNOHANG) != 0) {
    sketchpid = 0;
    return false;
  }

  return true;
}

void sketchSendData() {
  char keysstr[keysSize+1] = {};
  for (unsigned i = 0; i < keysSize; ++i)
    keysstr[i] = '0'+sf::Keyboard::isKeyPressed(keys[i]);

  char data[1024];
  sprintf(data, "%d %d %d %d %d %d %s\n",
          frameCount, windowWidth, windowHeight, mouseX, mouseY, mouseState, keysstr);

  write(sketchin[WRITE], data, strlen(data));
}

void sketchReceiveData() {
  // TODO IMPORTANT(naum): Multithread this
  std::string data;

  while (1) {
    data = "";
    while (1) {
      char c;
      int bytesread = read(sketchout[READ], &c, 1);
      if (bytesread <= 0) return;
      data += c;
      if (c == '\n') break;
    }

    int type;
    std::istringstream cmd(data);
    cmd >> type;

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
      sf::RectangleShape rect { { w, h } };
      rect.setFillColor(strokeColor);
      rect.setOrigin(0.0f, h / 2.0f);
      rect.setPosition(x0, y0);
      rect.rotate(angle);
      window.draw(rect);

      // Round ends
      sf::CircleShape circle { radius };
      circle.setFillColor(strokeColor);
      circle.setOrigin(radius, radius);
      circle.setPosition(x0, y0);
      window.draw(circle);

      circle.setPosition(x1, y1);
      window.draw(circle);
    }

    if (type == COMMAND_RECT) {
      int x, y, w, h;
      cmd >> x >> y >> w >> h;

      sf::RectangleShape rect;

      // Stroke
      rect.setFillColor(strokeColor);

      // Top
      rect.setSize({ (float)w, strokeThickness });
      rect.setPosition(x, y - strokeThickness);
      window.draw(rect);

      // Bottom
      rect.setPosition(x, y + h);
      window.draw(rect);

      // Left
      rect.setSize({ strokeThickness, (float)h });
      rect.setPosition(x - strokeThickness, y);
      window.draw(rect);

      // Right
      rect.setPosition(x + w, y);
      window.draw(rect);

      // Round corners
      float radius = strokeThickness;
      sf::CircleShape circle { radius };
      circle.setFillColor(strokeColor);
      circle.setOrigin(radius, radius);
      circle.setPosition(x, y);
      window.draw(circle);

      circle.setPosition(x + w, y);
      window.draw(circle);

      circle.setPosition(x + w, y + h);
      window.draw(circle);

      circle.setPosition(x, y + h);
      window.draw(circle);

      // Fill
      rect.setSize({ (float)w, (float)h });
      rect.setFillColor(fillColor);
      rect.setPosition(x, y);
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

}
