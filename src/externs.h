#ifndef CODESKETCH_CORE_EXTERNS_H
#define CODESKETCH_CORE_EXTERNS_H

#include <experimental/filesystem>

#include <map>
#include <queue>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace fs = std::experimental::filesystem;

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

// Text
extern std::map<int, TTF_Font*> fontCache;
extern SDL_Color textColor;
extern int textSize;

// Filesystem
extern std::queue<std::string> shellHistory;
extern fs::path shellRoot;
}

#endif
