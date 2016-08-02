#include <experimental/filesystem>

#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text.h"

namespace codesketch {

std::map<int, TTF_Font*> fontCache;
SDL_Color textColor = {255, 255, 255, 255};
int textSize = 16;

bool textInit() {
  if (TTF_Init() == -1) {
    // TODO(naum): print error
    return false;
  }
  return true;
}

void textQuit() {
  // Destroy fonts
  for (auto it : fontCache)
    TTF_CloseFont(it.second);
  TTF_Quit();
}

// TODO(naum): Return texture to avoid rerasterization
void textRender(const std::string& text, int x, int y, int size, SDL_Color color) {
  TTF_Font* font = nullptr;

  if (fontCache.find(size) == fontCache.end()) {
    // Load font
    font = TTF_OpenFont((shellRoot / "fonts/prstartk.ttf").c_str(), size);
    if (font == nullptr) {
      printf("Error: %s\n", TTF_GetError());
      return;
    }

    fontCache[size] = font;
  } else {
    font = fontCache[size];
  }

  SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_Rect renderRect = { x, y, surface->w, surface->h };
  SDL_RenderCopy(renderer, texture, nullptr, &renderRect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void textSetSize(int size) {
  if (size > 0)
    textSize = size;
}

void textSetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  textColor = {r, g, b, a};
}

}
