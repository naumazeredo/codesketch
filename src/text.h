#ifndef CODESKETCH_CORE_TEXT_H
#define CODESKETCH_CORE_TEXT_H

#include <string>

#include "externs.h"

namespace codesketch {

bool textInit();
void textQuit();
void textRender(const std::string&, int, int, int size = textSize);

}

#endif
