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
extern sf::ContextSettings windowSettings;

extern const std::string windowTitle;
extern const u32 windowStyle;

extern const int defaultWindowWidth, defaultWindowHeight, defaultWindowFramerate;
extern int windowWidth, windowHeight, windowFramerate;
extern bool isRunning;

extern fs::path binPath;

// Text
extern sf::Color textColor;
extern u32 textSize;

// Filesystem
extern fs::path shellRoot;
extern std::string shellInput;

// Sketch
struct SketchState {
  sf::Color fillColor, strokeColor;
  float strokeThickness;
};

extern SketchState sketchState;
extern fs::path sketchPath;

}

#endif
