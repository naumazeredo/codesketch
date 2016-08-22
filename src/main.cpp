#include <algorithm>

#include <SFML/Graphics.hpp>

#include "externs.h"
#include "text.h"
#include "shell.h"
//#include "sketch.h"

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
  window.clear();
  shellAddHistory("Type help for help!");

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      /*
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
      */
    }

    /*
    keystate = SDL_GetKeyboardState(nullptr);
    mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    */

    /*
    if (sketchIsRunning()) {
      // If sketch is running, I/O the sketch
      sketchSendData();
      sketchReceiveData();
      frameCount++;
    } else {
    */
      // If sketch is not running, show the shell
      window.clear();

      shellDraw();
      /*
    }
    */

    window.display();
  }

  /*
  SDL_Delay(250);
  */
}

}

int main(int argc, char** argv) {
  codesketch::init();
  codesketch::run();
}
