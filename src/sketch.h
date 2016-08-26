#ifndef CODESKETCH_CORE_SKETCH_H
#define CODESKETCH_CORE_SKETCH_H

#include <string>

#include <SFML/Window.hpp>

#define READ  0
#define WRITE 1

namespace codesketch {

const sf::Keyboard::Key keys[] = {
  // Letters
  sf::Keyboard::A, sf::Keyboard::B, sf::Keyboard::C, sf::Keyboard::D,
  sf::Keyboard::E, sf::Keyboard::F, sf::Keyboard::G, sf::Keyboard::H,
  sf::Keyboard::I, sf::Keyboard::J, sf::Keyboard::K, sf::Keyboard::L,
  sf::Keyboard::M, sf::Keyboard::N, sf::Keyboard::O, sf::Keyboard::P,
  sf::Keyboard::Q, sf::Keyboard::R, sf::Keyboard::S, sf::Keyboard::T,
  sf::Keyboard::U, sf::Keyboard::V, sf::Keyboard::W, sf::Keyboard::X,
  sf::Keyboard::Y, sf::Keyboard::Z,

  // Numbers
  sf::Keyboard::Num0, sf::Keyboard::Num1,
  sf::Keyboard::Num2, sf::Keyboard::Num3,
  sf::Keyboard::Num4, sf::Keyboard::Num5,
  sf::Keyboard::Num6, sf::Keyboard::Num7,
  sf::Keyboard::Num8, sf::Keyboard::Num9,

  // Extra keys
  sf::Keyboard::LControl, sf::Keyboard::RControl,
  sf::Keyboard::LShift, sf::Keyboard::RShift,

  sf::Keyboard::LAlt, sf::Keyboard::RAlt,

  //sf::Keyboard::LSystem, sf::Keyboard::RSystem,
  //sf::Keyboard::Menu,

  sf::Keyboard::LBracket, sf::Keyboard::RBracket,

  sf::Keyboard::SemiColon,
  sf::Keyboard::Comma,
  sf::Keyboard::Period,
  sf::Keyboard::Quote,
  sf::Keyboard::Slash,
  sf::Keyboard::BackSlash,
  sf::Keyboard::Tilde,
  sf::Keyboard::Equal,
  sf::Keyboard::Dash,

  sf::Keyboard::Space,
  sf::Keyboard::Return,
  sf::Keyboard::BackSpace,
  sf::Keyboard::Tab,

  sf::Keyboard::PageUp, sf::Keyboard::PageDown,
  sf::Keyboard::Home, sf::Keyboard::End,
  sf::Keyboard::Insert, sf::Keyboard::Delete,

  sf::Keyboard::Add, sf::Keyboard::Subtract,
  sf::Keyboard::Multiply, sf::Keyboard::Divide,

  sf::Keyboard::Left, sf::Keyboard::Right,
  sf::Keyboard::Up,   sf::Keyboard::Down,

  sf::Keyboard::Numpad0, sf::Keyboard::Numpad1,
  sf::Keyboard::Numpad2, sf::Keyboard::Numpad3,
  sf::Keyboard::Numpad4, sf::Keyboard::Numpad5,
  sf::Keyboard::Numpad6, sf::Keyboard::Numpad7,
  sf::Keyboard::Numpad8, sf::Keyboard::Numpad9,

  sf::Keyboard::F1,  sf::Keyboard::F2,
  sf::Keyboard::F3,  sf::Keyboard::F4,
  sf::Keyboard::F5,  sf::Keyboard::F6,
  sf::Keyboard::F7,  sf::Keyboard::F8,
  sf::Keyboard::F9,  sf::Keyboard::F10,
  sf::Keyboard::F11, sf::Keyboard::F12
};

const int keysSize = sizeof(keys)/sizeof(keys[0]);

enum {
  COMMAND_FRAMEEND,

  COMMAND_BACKGROUND,

  COMMAND_POINT,
  COMMAND_LINE,
  COMMAND_RECT,
  COMMAND_CIRCLE,

  COMMAND_FILLCOLOR,

  COMMAND_STROKECOLOR,
  COMMAND_STROKETHICKNESS,

  COMMAND_TEXT,
  COMMAND_TEXTSIZE,

  // XXX(naum): Not implemented
  COMMAND_ELLIPSE,

  COMMAND_CAMERA,
  COMMAND_WINDOW,

  COMMAND_EXIT,

  COMMAND_NUM
};

bool sketchOpen(const std::string&);
void sketchClose();
bool sketchIsRunning();
void sketchRun();

}

#endif
