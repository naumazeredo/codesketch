#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "externs.h"
#include "text.h"
#include "sketch.h"
#include "shell.h"

namespace codesketch {

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int windowWidth = 640, windowHeight = 480;
bool isRunning = true;

int mouseX, mouseY;
Uint32 mouseState;

int frameCount = 0;

void init() {
  // TODO(naum): treat errors
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);

  textInit();

  // Create window
  window = SDL_CreateWindow(
    "Code Sketch",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    windowWidth, windowHeight, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void shutdown() {
  textQuit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void run() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  shellAddHistory("Type help for help!");

  SDL_StartTextInput();

  unsigned ticks = SDL_GetTicks();
  isRunning = true;
  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        isRunning = false;
      }

      if (sketchIsRunning()) {
        if (event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_ESCAPE) {
          sketchClose();
        }
      } else {
        if (event.type == SDL_KEYDOWN) {
          if (event.key.keysym.sym == SDLK_BACKSPACE) {
            shellBackspace();
          } else if (event.key.keysym.sym == SDLK_RETURN) {
            shellParseInput();
          } else if (event.key.keysym.sym == SDLK_l and SDL_GetModState() & KMOD_CTRL) {
            shellClearHistory();
          }
        } else if (event.type == SDL_TEXTINPUT) {
          shellAddInput(event.text.text);
        }
      }
    }

    keystate = SDL_GetKeyboardState(nullptr);
    mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    if (sketchIsRunning()) {
      // If sketch is running, I/O the sketch
      sketchSendData();
      sketchReceiveData();
    } else {
      // If sketch is not running, show the shell
      SDL_RenderClear(renderer);

      shellDraw();
    }

    SDL_RenderPresent(renderer);

    // TODO(naum): verify if running slower than 60 FPS
    SDL_Delay(std::max(1000/60 - (SDL_GetTicks() - ticks), (Uint32)0));
    ticks = SDL_GetTicks();

    frameCount++;
  }

  SDL_Delay(250);
}

}

int main(int argc, char** argv) {
  codesketch::init();
  codesketch::run();
  codesketch::shutdown();
}
