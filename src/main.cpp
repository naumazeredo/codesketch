#include <algorithm>

#include <SFML/Graphics.hpp>

#include "externs.h"
#include "text.h"
//#include "sketch.h"
//#include "shell.h"

namespace codesketch {

sf::ContextSettings settings;
sf::RenderWindow window;

int windowWidth = 640, windowHeight = 480;

int mouseX, mouseY;
//u32 mouseState;

int frameCount;

void init() {
  // TODO(naum): treat errors
  // Initialize settings
  settings.antialiasingLevel = 8;

  // Initialize Window
  window.create(
    sf::VideoMode(windowWidth, windowHeight),
    "CodeSketch",
    sf::Style::Default,
    settings
  );

  window.setFramerateLimit(60);

  // Initialize text
  textInit();
}

void run() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    textRender("Test", 100, 100);
    window.display();
  }

  /*
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
      frameCount++;
    } else {
      // If sketch is not running, show the shell
      SDL_RenderClear(renderer);

      shellDraw();
    }

    SDL_RenderPresent(renderer);

    // TODO(naum): verify if running slower than 60 FPS
    SDL_Delay(std::max(1000/60 - (SDL_GetTicks() - ticks), (Uint32)0));
    ticks = SDL_GetTicks();
  }

  SDL_Delay(250);
  */
}

}

int main(int argc, char** argv) {
  codesketch::init();
  codesketch::run();
}
