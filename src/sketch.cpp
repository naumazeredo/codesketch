#include "sketch.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <cstdio>
#include <string>

#include <SDL2/SDL.h>

#include "externs.h"
#include "text.h"

namespace codesketch {

const Uint8* keystate = nullptr;

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
    if (!SDL_IsTextInputActive())
      SDL_StartTextInput();
    sketchpid = 0;
    return false;
  }

  if (SDL_IsTextInputActive())
    SDL_StopTextInput();
  return true;
}

void sketchSendData() {
  char keysstr[keysSize+1] = {};
  for (unsigned i = 0; i < keysSize; ++i) keysstr[i] = '0'+keystate[keys[i]];

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

    char type;
    std::istringstream cmd(data);
    cmd >> type;

    if (type == COMMAND_BACKGROUND) {
      int nr, ng, nb;
      cmd >> nr >> ng >> nb;

      Uint8 r, g, b, a;
      SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
      SDL_SetRenderDrawColor(renderer, nr, ng, nb, 255);
      SDL_RenderClear(renderer);
      SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }

    if (type == COMMAND_STROKECOLOR) {
      int r, g, b, a;
      cmd >> r >> g >> b >> a;
      SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }

    if (type == COMMAND_POINT) {
      int x, y;
      cmd >> x >> y;
      SDL_RenderDrawPoint(renderer, x, y);
    }

    if (type == COMMAND_LINE) {
      int x0, y0, x1, y1;
      cmd >> x0 >> y0 >> x1 >> y1;
      SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
    }

    if (type == COMMAND_RECT) {
      int x, y, w, h;
      cmd >> x >> y >> w >> h;
      SDL_Rect rect = {x, y, w, h};
      SDL_RenderDrawRect(renderer, &rect);
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
