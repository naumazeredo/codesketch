#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <cstdio>
#include <string>
#include <map>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace codesk {

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

map<int, TTF_Font*> fontCache;
SDL_Color fontColor = {255, 255, 255};
int fontSize = 16;

int windowWidth = 640, windowHeight = 480;

bool isRunning = true;

const Uint8* keystate = nullptr;
int mouseX, mouseY;
Uint32 mouseState;

const int keys[] = {
  // Letters
  SDL_SCANCODE_A, SDL_SCANCODE_B, SDL_SCANCODE_C, SDL_SCANCODE_D,
  SDL_SCANCODE_E, SDL_SCANCODE_F, SDL_SCANCODE_G, SDL_SCANCODE_H,
  SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L,
  SDL_SCANCODE_M, SDL_SCANCODE_N, SDL_SCANCODE_O, SDL_SCANCODE_P,
  SDL_SCANCODE_Q, SDL_SCANCODE_R, SDL_SCANCODE_S, SDL_SCANCODE_T,
  SDL_SCANCODE_U, SDL_SCANCODE_V, SDL_SCANCODE_W, SDL_SCANCODE_X,
  SDL_SCANCODE_Y, SDL_SCANCODE_Z,

  // Numbers
  SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
  SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7,
  SDL_SCANCODE_8, SDL_SCANCODE_9,

  // Arrows
  SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,

  SDL_SCANCODE_RETURN, SDL_SCANCODE_BACKSPACE,
  SDL_SCANCODE_SPACE, SDL_SCANCODE_DELETE,

  // F keys
  SDL_SCANCODE_F1, SDL_SCANCODE_F2, SDL_SCANCODE_F3, SDL_SCANCODE_F4,
  SDL_SCANCODE_F5, SDL_SCANCODE_F6, SDL_SCANCODE_F7, SDL_SCANCODE_F8,
  SDL_SCANCODE_F9, SDL_SCANCODE_F10, SDL_SCANCODE_F11, SDL_SCANCODE_F12,

  // Mod Keys
  SDL_SCANCODE_LSHIFT, SDL_SCANCODE_RSHIFT,
  SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL
};

const int keysSize = sizeof(keys)/sizeof(keys[0]);

enum {
  COMMAND_NOTHING,
  COMMAND_BACKGROUND,
  COMMAND_STROKECOLOR,
  COMMAND_POINT,
  COMMAND_LINE,
  COMMAND_RECT,

  // XXX(naum): Not implemented
  COMMAND_FILLCOLOR,
  COMMAND_STROKEWIDTH,
  COMMAND_CIRCLE,
  COMMAND_ELLIPSE,

  COMMAND_CAMERA,
  COMMAND_WINDOW,

  COMMAND_EXIT,

  COMMAND_NUM
};

// Sketch subprocess
#define READ  0
#define WRITE 1

pid_t sketchpid = 0;
int sketchin[2], sketchout[2], sketcherr[2];
int sketchstatus = 0;

void init() {
  // TODO(naum): treat errors
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  // Create window
  window = SDL_CreateWindow(
    "Code Sketch",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    windowWidth, windowHeight, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void shutdown() {
  // Destroy fonts
  for (auto it : fontCache)
    TTF_CloseFont(it.second);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
}

void renderText(const char* text, int x, int y) {
  TTF_Font* font = nullptr;

  if (fontCache.find(fontSize) == fontCache.end()) {
    // Load font
    font = TTF_OpenFont("fonts/8bitwonder.ttf", fontSize);
    if (font == nullptr) {
      printf("Error: %s\n", TTF_GetError());
      return;
    }

    fontCache[fontSize] = font;
  } else {
    font = fontCache[fontSize];
  }

  SDL_Surface* surface = TTF_RenderText_Solid(font, text, fontColor);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect renderRect = { x, y, surface->w, surface->h };
  SDL_RenderCopy(renderer, texture, nullptr, &renderRect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

bool isSketchRunning() {
  if (sketchpid == 0) return false;

  sketchstatus = 0;
  if (waitpid(sketchpid, &sketchstatus, WNOHANG) != 0) {
    printf("Sketch exited!\n");
    sketchpid = 0;
    return false;
  }

  return true;
}

bool openSketch(const char* name) {
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

    if (execl(name, name, nullptr) == -1) {
      // Exec fail: kill child!
      kill(getpid(), SIGKILL);
    }
  } else if (sketchpid > 0) {
    // Parent process
    close(sketchin[READ]);
    close(sketchout[WRITE]);
    close(sketcherr[WRITE]);

    fcntl(sketchout[READ], F_SETFL, fcntl(sketchout[READ], F_GETFL, 0) | O_NONBLOCK);
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
  if (isSketchRunning()) {
    printf("Sketch %s opened!\n", name);
    return true;
  } else {
    printf("Sketch %s could not be opened!\n", name);
    return false;
  }
}

void killSketch() {
  kill(sketchpid, SIGKILL);
  close(sketchin[WRITE]);
  close(sketchout[READ]);
  close(sketcherr[READ]);
}

void sendData() {
  char keysstr[keysSize+1] = {};
  for (unsigned i = 0; i < keysSize; ++i) keysstr[i] = '0'+keystate[keys[i]];

  char data[1024];
  sprintf(data, "%d %d %d %d %d %s\n", windowWidth, windowHeight, mouseX, mouseY, mouseState, keysstr);

  write(sketchin[WRITE], data, strlen(data));
}

void receiveData() {
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

void run() {
  //openSketch("bin/sketch.out");

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  unsigned ticks = SDL_GetTicks();

  isRunning = true;
  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        isRunning = false;
      } else if (event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_ESCAPE) {
        // Kill sketch on ESC
        if (isSketchRunning()) killSketch();
      }
    }

    keystate = SDL_GetKeyboardState(nullptr);
    mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    // If loaded sketch
    if (isSketchRunning()) {
      sendData();
      receiveData();
    } else {
      SDL_RenderClear(renderer);

      renderText("Code Sketch", 20, 20);
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(max(1000/60 - (SDL_GetTicks() - ticks), (Uint32)0));
    ticks = SDL_GetTicks();
  }
}

}

int main(int argc, char** argv) {
  codesk::init();
  codesk::run();
  codesk::shutdown();
}
