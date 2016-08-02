# Code Sketch

Simple platform to teach C and C++ easily.

*Still in development*
*Linux-only by now*

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
- Very mechanical and little creative

So, the usual programming learning environment seems to discourage students
instead of motivating them. Of course many still love from the very beginning,
but why not the majority of them?

Using the [Processing](https://processing.org) idea, as a flexible software
sketchbook, I'm developing a C/C++ sketchbook, named **Code Sketch**.

## TODO

- [x] Simple code sketch and sketchbook sources
- [x] Simple Makefile
- [ ] Implement text rendering
- [x] Create simple GUI in codesketch to list and run sketches
- [x] Refactor code sketch for alfa version
- [ ] Implement drawing primitives
  - [x] Points Built-in
  - [x] Lines Built-in
  - [x] Rectangles Built-in
  - [ ] Stroke width
  - [ ] Fill color
  - [ ] Points
  - [ ] Lines
  - [ ] Rectangles
  - [ ] Ellipses
- [ ] Camera functionality
- [ ] Implement window resizing (?)
- [ ] Windows porting
- [ ] Write README blank/incomplete sections
- [ ] IMPORTANT: parallelize subprocess reading to avoid crash

## Learning with Code Sketch

### Install Dependencies

`$ sudo apt-get install libsdl2-2.0-0 libsdl2-ttf-2.0-0`

### Begin to code

Inside *sketches* folder, create a new file `mysketch.c/cpp` and type in:

```cpp
#include "sketch.h"

void setup() {
}

void draw() {
}
```
Your sketch **must** declare `void setup()` and `void draw()` functions, and
that's the only restriction!

`setup` function is called only once when your sketch loads;

`draw` function is called every update (usually 60 times per second).

You have access to a Code Sketch exclusive API and to everything you need from
C/C++ libraries. **The only thing you can't do input and output, since Code
Sketch uses this to communicate between Code Sketch and your sketch**.

#### API

```cpp
// Window size (modifying these don't alter the window size)
int windowWidth, windowHeight

// Mouse position
int mouseX, mouseY;

// Mouse buttons
// Verify if mouse button (left or right) are pressed.
// return 1 if is pressed, 0 otherwise
int mousePressedLeft();
int mousePressedRight();

// Keyboard
// Verify if key is pressed
// return 1 if is pressed, 0 otherwise
// Keys:
// KEY_A ~ KEY_Z, KEY_0 ~ KEY_9, KEY_F1 ~ KEY_F12
// KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
// KEY_RETURN, KEY_BACKSPACE, KEY_SPACE, KEY_DELETE
// KEY_SHIFT, KEY_CTRL
int keyPressed(int key);

// Drawing

// All colors MUST have values between 0 and 255, inclusive.
// r = red, g = green, b = blue

// Clear the whole screen with the color
void background(int r, int g, int b);

// Change stroke color
void stroke(int r, int g, int b, int a);

// Draw a point/pixel in position (x, y)
void point(int x, int y);

// Draw a line with start at (x0, y0) and end at (x1, y1)
void line(int x0, int y0, int x1, int y1);

// Draw a rectangle with upper-left corner at (x0, y0), width w and height h
void rect(int x, int y, int w, int h);

// Text

// Render text t with upper-left corner at (x, y)
void text(const char* t, int x, int y);

// Change text size (s must be positive)
void textSize(int s);

// Change text color
void textColor(int r, int g, int b, int a);
```

See examples in *sketches* folder if you have any doubts on how to use the API.

Compile your code:

### Be creative!

Now you can begin to code everything you want and imagine!

Use some C or C++ material to follow you through this whole new universe (some
good ones are [Learn CPP](http://www.learncpp.com/) and [cplusplus.com](http://www.cplusplus.com/doc/tutorial/). Just remember to don't use **cin/cout**, **scanf/printf** or **read/write**).

Happy coding!

## Contributing

### Dependencies

GCC>=6.1 (or some C++ compiler with C++1z File System support): http://askubuntu.com/questions/746369/how-can-i-install-and-use-gcc-6-on-xenial

SDL2 libs: `sudo apt install libsdl2-dev libsdl2-ttf-dev`

## Known bugs

- When pipe reaches the size limit program stops working. Solution: parallelize
    subprocess reading
