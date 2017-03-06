# Code Sketch

Simple platform to teach C and C++ easily.

Runs on Windows and Linux!

## Idea

The basic idea is that a proper environment eases learning your first
programming language.

Since C/C++ is sometimes the first programming language in many universities, I,
as a self taught programmer, gathered the learning difficulties students have in
the very beginning and tried to solve them all with this platform. The issues I
noticed are:

- No visual feedback
- Too many lines for very basic programs
- Too many library includes for very basic programs
- Too mechanical and little creative

So, the usual programming learning environment seems to discourage students
instead of motivating them. Of course many still love from the very beginning,
but why not the majority of them?

Using the [Processing](https://processing.org) idea, as a flexible software
sketchbook, I'm developing a C/C++ sketchbook, named **Code Sketch**.

## Screenshots

![Code Sketch
shell](https://raw.githubusercontent.com/naumazeredo/codesketch/images/images/codesketch.png)

![Shell help]( https://raw.githubusercontent.com/naumazeredo/codesketch/images/images/help.png)

![Example](https://raw.githubusercontent.com/naumazeredo/codesketch/images/images/example_logo.png)

![Tetris](https://raw.githubusercontent.com/naumazeredo/codesketch/images/images/example_tetris.png)


## Learning with Code Sketch

OBS.: On Windows, CodeSketch is _sometimes_ blocked by Smart Screen or
anti-virus.

### Begin to code

Inside *sketches* folder, create a new file `mysketch.c/cpp` and type in:

```cpp
#include "sketch.h"

void setup() {
}

void draw() {
}
```
Your sketch **must** declare `void setup()` and `void draw()` functions.

`setup` function is called only once when your sketch loads.

`draw` function is called every update (usually 60 times per second).

You have access to a Code Sketch exclusive API and to everything you need from
C/C++ libraries. **The only thing you can't do is input and output. Code Sketch
uses standard I/O to communicate between Code Sketch and your sketch**.

#### API

```cpp
// Frame counter since sketch startup
int frameCounter;

// Window size (modifying these don't alter the window size)
int windowWidth, windowHeight;

// Mouse position
int mouseX, mouseY;

// Mouse

// Verify if a mouse button is held pressed.
// return 0 if is up, > 0 otherwise.
// Buttons:
// MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE,
// MOUSE_X1, MOUSE_X2
int mouseDown(int button);

// Keyboard

// Verify if key is held down.
// return 0 if is up, > 0 otherwise.
// Keys:
// KEY_A ~ KEY_Z, KEY_0 ~ KEY_9, KEY_F1 ~ KEY_F12
// KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
// KEY_RETURN, KEY_BACKSPACE, KEY_SPACE, KEY_DELETE
// KEY_SHIFT, KEY_CTRL
int keyDown(int key);

// Drawing

// All colors MUST have values between 0 and 255, inclusive.
// Colors not between 0 and 255 will be truncated.
// r = red, g = green, b = blue, a = alpha (default: 255)

// Clear the whole screen with the color
void background(int r, int g, int b);

// Primitives

// Draw a point/pixel in position (x, y)
void point(int x, int y);

// Draw a line with start at (x0, y0) and end at (x1, y1)
void line(int x0, int y0, int x1, int y1);

// Draw a rectangle with upper-left corner at (x0, y0), width w and height h
// (w and h must be positive)
void rect(int x, int y, int w, int h);

// Draw a circle with center at (x, y) and radius r
void circle(int x, int y, int r);

// Color

// Change fill color
void fill(int r, int g, int b, int a);

// Stroke
// Points will have the stroke thickness as size (min 1)
// Lines will have the stroke thickness as height (min 1)
// Rects and circles will have an outline with stroke thickness

// Change stroke color
void stroke(int r, int g, int b, int a);

// Change stroke thickness
void strokeThickness(int t);

// Text

// Render text format (using extra variables, like printf) with upper-left corner at (x, y)
// The text will have the fill color as it's color
void text(int x, int y, const char* format, ...);

// Change text size (s must be positive)
void textSize(int s);

// Camera

// Move top-left camera corner to (x, y). Starts at (0, 0)
// (note: moving camera without resending drawing doesn't work as you would expect.
// This will probably be modified in the future.)
void camera(int x, int y);

// Debug

// Print debug format (using extra variables, like printf)
void debug(const char* format, ...);

// Exit

// Terminate sketch execution
void exit();

// Setup exclusive commands

// Enables window antialiasing (to avoid pixelated graphics)
void smooth();

// Changes window framerate to r
void framerate(int r);

// Changes window size to (w, h)
void window(int w, int h);

```

See examples in *sketches* folder if you have any doubts on how to use the API.

Compile your code. (Linux: `g++ mysketch.c -o mysketch.out`. Windows: compile on
any IDE) (on Windows you can put a compile flag -mwindows to disable the
console on run).

Run Code Sketch binary and type: `> run sketches/mysketch.out` or `> run
sketches/mysketch.exe` (you can find your binary using `ls` and `cd` commands).

(You can take screenshots by pressing F12. If you can't show your sketch
running, at least take a picture :wink:)

### Be creative!

Now you can begin to code everything you want and imagine!

Use some C or C++ material to follow you through this whole new universe (some
good ones are [Learn CPP](http://www.learncpp.com/) and [cplusplus.com](http://www.cplusplus.com/doc/tutorial/). Just remember to don't use **cin/cout**, **scanf/printf** or **read/write**).

Happy coding!

## Contributing

### Dependencies

#### Ubuntu 16.04 or later

GCC C++: `sudo apt install g++ binutils`

SFML: `sudo apt install libsfml-dev`

Boost Filesystem: `sudo apt install libboost-filesystem-dev`

## Known bugs/problems

- SFML doesn't recognize keyboard layout from OS.
- Windows version is blocked by SmartScreen and some anti-viruses.
- May run suboptimally in case of large inputs.
- Defines on _text_ and _debug_ may not be the best option.
- Inserting `\n` on text and debug can cause issues. (!)

## TODO

- [x] Simple code sketch and sketchbook sources
- [x] Simple Makefile
- [x] Implement text rendering
  - [ ] Dynamic font loading (by sketches) (?)
- [x] Create simple GUI in codesketch to list and run sketches
- [x] Refactor code sketch for alfa version
- [x] Implement drawing primitives
  - [x] Points
  - [x] Lines
  - [x] Rectangles
  - [x] Circles
  - [ ] Ellipses (?)
- [x] Camera functionality
  - [ ] Move camera without redrawing everything to work
- [ ] Drawing rotate/scale/zoom
- [ ] Shell
  - [x] Basic commands
  - [ ] Text wrapping
  - [ ] Blinking cursor
  - [ ] Caret
  - [ ] Colors
- [x] Window resizing
- [x] Port to Windows
- [ ] Add version and setup process (instead of sending same data to setup)
- [ ] Verify if sketch is running suboptimally (less than 60fps. May happen
  with large input)
