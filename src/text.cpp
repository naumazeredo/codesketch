#include "text.h"

namespace codesketch {

sf::Font font;
u32 textSize = 16;

bool textInit() {
  std::string path = (binPath / "../fonts/UbuntuMono-B.ttf").string();
  if (!font.loadFromFile(path)) {
    // TODO(naum): print error
    return false;
  }
  return true;
}

void textRender(const std::string& text, int x, int y, u32 size, sf::Color color) {
  sf::Text t { text, font, size };
  t.setColor(color);
  t.setPosition(x, y);
  window.draw(t);
}

void textSetSize(u32 size) {
  textSize = size;
}

}
