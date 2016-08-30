#ifndef CODESKETCH_CORE_TEXT_H
#define CODESKETCH_CORE_TEXT_H

#include <string>

#include <SFML/Graphics.hpp>

#include "externs.h"

namespace codesketch {

bool textInit();
void textQuit();
void textRender(const std::string&, int, int, u32 size = textSize, sf::Color color = sketchState.fillColor);
void textSetSize(u32);

}

#endif
