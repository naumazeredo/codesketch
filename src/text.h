#ifndef CODESKETCH_CORE_TEXT_H
#define CODESKETCH_CORE_TEXT_H

#include <string>

#include "externs.h"

namespace codesketch {

namespace colors {

const SDL_Color white = {0xff, 0xff, 0xff, 0xff};
const SDL_Color black = {0x00, 0x00, 0x00, 0xff};
const SDL_Color red   = {0xff, 0x00, 0x00, 0xff};
const SDL_Color green = {0x00, 0xff, 0x00, 0xff};
const SDL_Color blue  = {0x00, 0x00, 0xff, 0xff};

}

bool textInit();
void textQuit();
void textRender(const std::string&, int, int, int size = textSize, SDL_Color color = textColor);
void textSetSize(int);
void textSetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

}

#endif
