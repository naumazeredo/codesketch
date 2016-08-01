#include "sketch.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <cstdio>
#include <string>

#include <SDL2/SDL.h>

#include "externs.h"

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

  usleep(1);
  return sketchIsRunning();
}

void sketchClose() {
  kill(sketchpid, SIGKILL);
  close(sketchin[WRITE]);
  close(sketchout[READ]);
  close(sketcherr[READ]);
}

bool sketchIsRunning() {
  if (sketchpid == 0) return false;

  sketchstatus = 0;
  if (waitpid(sketchpid, &sketchstatus, WNOHANG) != 0) {
    printf("Sketch exited!\n");
    sketchpid = 0;
    return false;
  }

  return true;
}

void sketchSendData() {
  char keysstr[keysSize+1] = {};
  for (unsigned i = 0; i < keysSize; ++i) keysstr[i] = '0'+keystate[keys[i]];

  char data[1024];
  sprintf(data, "%d %d %d %d %d %s\n", windowWidth, windowHeight, mouseX, mouseY, mouseState, keysstr);

  write(sketchin[WRITE], data, strlen(data));
}

void sketchReceiveData() {
  char data[1024];
  memset(data, 0, sizeof(data));

  while (1) {
    int bytesread = read(sketchout[READ], data, 1024);
    if (bytesread <= 0) break;

    char* command = strtok(data, "\n");
    while (command != nullptr) {
      char type;
      int x, y, z, w;
      sscanf(command, "%c %d %d %d %d", &type, &x, &y, &z, &w);

      if (type == COMMAND_BACKGROUND) {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(renderer, x, y, z, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
      }
      if (type == COMMAND_STROKECOLOR) SDL_SetRenderDrawColor(renderer, x, y, z, w);
      if (type == COMMAND_POINT) SDL_RenderDrawPoint(renderer, x, y);
      if (type == COMMAND_LINE) SDL_RenderDrawLine(renderer, x, y, z, w);
      if (type == COMMAND_RECT) {
        SDL_Rect rect = {x, y, z, w};
        SDL_RenderDrawRect(renderer, &rect);
      }

      command = strtok(nullptr, "\n");
    }
  }
}

}
