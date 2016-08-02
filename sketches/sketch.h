#include <stdio.h>
#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;

enum {
  COMMAND_NOTHING,

  COMMAND_BACKGROUND,

  COMMAND_STROKECOLOR,
  COMMAND_POINT,
  COMMAND_LINE,
  COMMAND_RECT,

  COMMAND_TEXT,
  COMMAND_TEXTSIZE,
  COMMAND_TEXTCOLOR,

  // XXX(naum): Not implemented
  COMMAND_FILLCOLOR,
  COMMAND_STROKEWIDTH,
  COMMAND_CIRCLE,
  COMMAND_ELLIPSE,

  COMMAND_CAMERA,
  COMMAND_WINDOW,

  COMMAND_EXIT,

  COMMAND_NUM
};

enum {
  // Letters
  KEY_A, KEY_B, KEY_C, KEY_D,
  KEY_E, KEY_F, KEY_G, KEY_H,
  KEY_I, KEY_J, KEY_K, KEY_L,
  KEY_M, KEY_N, KEY_O, KEY_P,
  KEY_Q, KEY_R, KEY_S, KEY_T,
  KEY_U, KEY_V, KEY_W, KEY_X,
  KEY_Y, KEY_Z,

  // Numbers
  KEY_0, KEY_1, KEY_2, KEY_3,
  KEY_4, KEY_5, KEY_6, KEY_7,
  KEY_8, KEY_9,

  // Arrows
  KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,

  KEY_RETURN, KEY_BACKSPACE,
  KEY_SPACE, KEY_DELETE,

  // F keys
  KEY_F1, KEY_F2, KEY_F3, KEY_F4,
  KEY_F5, KEY_F6, KEY_F7, KEY_F8,
  KEY_F9, KEY_F10, KEY_F11, KEY_F12,

  // Mod Keys
  KEY_LSHIFT, KEY_RSHIFT,
  KEY_LCTRL, KEY_RCTRL,

  KEY_SHIFT, KEY_CTRL,

  KEY_NUM
};

int windowWidth, windowHeight;
int mouseX, mouseY;
int mouseState_;
char keystate_[256];

// User declared functions
void setup();
void draw();

int main() {
  // Unbuffered stdout
  setbuf(stdout, 0);

  // Setup
  scanf("%d %d %d %d %d %s",
        &windowWidth, &windowHeight,
        &mouseX, &mouseY, &mouseState_,
        keystate_);
  setup();

  while (scanf("%d %d %d %d %d %s",
               &windowWidth, &windowHeight,
               &mouseX, &mouseY, &mouseState_,
               keystate_) != EOF) {
    draw();
  }

  return 0;
}

// Mouse and keyboard
int mousePressedLeft() {
  return !!(mouseState_ & 1);
}

int mousePressedRight() {
  return !!(mouseState_ & 2);
}

int keydown_(u8 key) {
  return keystate_[key]-'0';
}

int keyPressed(u8 key) {
  if (key == KEY_SHIFT) return keydown_(KEY_LSHIFT) | keydown_(KEY_RSHIFT);
  if (key == KEY_CTRL) return keydown_(KEY_LCTRL) | keydown_(KEY_RCTRL);
  return keydown_(key);
}

void background(u8 r, u8 g, u8 b) {
  printf("%c %d %d %d\n", COMMAND_BACKGROUND, r, g, b);
}

void stroke(u8 r, u8 g, u8 b, u8 a = 0xff) {
  printf("%c %d %d %d %d\n", COMMAND_STROKECOLOR, r, g, b, a);
}

void point(i32 x, i32 y) {
  printf("%c %d %d\n", COMMAND_POINT, x, y);
}

void line(i32 x0, i32 y0, i32 x1, i32 y1) {
  printf("%c %d %d %d %d\n", COMMAND_LINE, x0, y0, x1, y1);
}

void rect(i32 x, i32 y, u16 w, u16 h) {
  printf("%c %d %d %d %d\n", COMMAND_RECT, x, y, w, h);
}

void text(const char* t, i32 x, i32 y) {
  printf("%c %d %d %s\n", COMMAND_TEXT, x, y, t);
}

void textSize(u16 s) {
  printf("%c %d\n", COMMAND_TEXTSIZE, s);
}

void textColor(u8 r, u8 g, u8 b, u8 a = 0xff) {
  printf("%c %d %d %d %d\n", COMMAND_TEXTCOLOR, r, g, b, a);
}

// XXX(naum): Not implemented

void fill(u8 r, u8 g, u8 b, u8 a = 0xff) {
  printf("%c %d %d %d %d\n", COMMAND_FILLCOLOR, r, g, b, a);
}

