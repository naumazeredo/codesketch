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

  SDL_StartTextInput();
  std::string textInput;

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
          if (event.key.keysym.sym == SDLK_BACKSPACE and textInput.length() > 0) {
            textInput.pop_back();
          } else if (event.key.keysym.sym == SDLK_RETURN) {
            shellParse(textInput);
            textInput.clear();
          }
        } else if (event.type == SDL_TEXTINPUT) {
          textInput += event.text.text;
        }
      }
    }

    keystate = SDL_GetKeyboardState(nullptr);
    mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    // If loaded sketch
    if (sketchIsRunning()) {
      sketchSendData();
      sketchReceiveData();
    } else {
      SDL_RenderClear(renderer);

      textRender("Code Sketch", 20, 20, 32);
      if (textInput.length() > 0) textRender(textInput.c_str(), 20, 60);
    }

    SDL_RenderPresent(renderer);

    // TODO(naum): verify if running slower than 60 FPS
    SDL_Delay(std::max(1000/60 - (SDL_GetTicks() - ticks), (Uint32)0));
    ticks = SDL_GetTicks();
  }
}

}

int main(int argc, char** argv) {
  codesketch::init();
  codesketch::run();
  codesketch::shutdown();
}
