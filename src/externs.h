#ifndef CODESKETCH_CORE_EXTERNS_H
#define CODESKETCH_CORE_EXTERNS_H

#include <SDL2/SDL.h>

namespace codesketch {

// Window
extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern int windowWidth;
extern int windowHeight;
extern bool isRunning;

extern int mouseX;
extern int mouseY;
extern Uint32 mouseState;

// Sketch
extern const Uint8* keystate;
}

#endif
