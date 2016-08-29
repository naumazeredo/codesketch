#ifndef CODESKETCH_CORE_EXTERNS_H
#define CODESKETCH_CORE_EXTERNS_H

#include <cstdint>
#include <boost/filesystem.hpp>

#include <SFML/Graphics.hpp>

namespace fs = boost::filesystem;

// Int types
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

namespace codesketch {

// Main
extern sf::RenderWindow window;

extern int windowWidth;
extern int windowHeight;
extern bool isRunning;

extern int mouseX;
extern int mouseY;
extern u8 mouseState;

extern int frameCount;

extern fs::path binPath;

// Text
extern sf::Color textColor;
extern u32 textSize;

// Filesystem
extern fs::path shellRoot;
extern std::string shellInput;

// Sketch
extern sf::Color fillColor;
extern fs::path sketchPath;

}

#endif
