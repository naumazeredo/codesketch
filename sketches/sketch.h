#include <stdio.h>
#include <stdint.h>

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
  KEY_0, KEY_1,
  KEY_2, KEY_3,
  KEY_4, KEY_5,
  KEY_6, KEY_7,
  KEY_8, KEY_9,

  // Modification keys
  KEY_LCTRL, KEY_RCTRL,
  KEY_LSHIFT, KEY_RSHIFT,
  KEY_LALT, KEY_RALT,

  // [, ]
  KEY_LBRACKET, KEY_RBRACKET,

  KEY_SEMICOLON,
  KEY_COMMA,
  KEY_PERIOD,
  KEY_QUOTE,
  KEY_SLASH,
  KEY_BACKSLASH,
  KEY_TILDE,
  KEY_EQUAL,
  KEY_DASH,

  KEY_SPACE,
  KEY_RETURN,
  KEY_BACKSPACE,
  KEY_TAB,

  KEY_PAGEUP, KEY_PAGEDOWN,
  KEY_HOME, KEY_END,
  KEY_INSERT, KEY_DELETE,

  KEY_ADD, KEY_SUBTRACT,
  KEY_MULTIPLY, KEY_DIVIDE,

  KEY_LEFT, KEY_RIGHT,
  KEY_UP,   KEY_DOWN,

  KEY_NUMPAD0, KEY_NUMPAD1,
  KEY_NUMPAD2, KEY_NUMPAD3,
  KEY_NUMPAD4, KEY_NUMPAD5,
  KEY_NUMPAD6, KEY_NUMPAD7,
  KEY_NUMPAD8, KEY_NUMPAD9,

  KEY_F1,  KEY_F2,
  KEY_F3,  KEY_F4,
  KEY_F5,  KEY_F6,
  KEY_F7,  KEY_F8,
  KEY_F9,  KEY_F10,
  KEY_F11, KEY_F12,

  KEY_SHIFT, KEY_CTRL, KEY_ALT,

  KEY_NUM
};

int frameCount;
int windowWidth, windowHeight;
int mouseX, mouseY;

int mouseState_;
char keystate_[256];
int running_;

// User declared functions
void setup();
void draw();

int main() {
  // Unbuffered stdout
  setbuf(stdout, 0);

  running_ = 1;

  // Setup
  scanf("%d %d %d %d %d %d %s",
        &frameCount,
        &windowWidth, &windowHeight,
        &mouseX, &mouseY, &mouseState_,
        keystate_);
  setup();
  printf("%d\n", COMMAND_FRAMEEND);

  while (running_ &&
         scanf("%d %d %d %d %d %d %s",
               &frameCount,
               &windowWidth, &windowHeight,
               &mouseX, &mouseY, &mouseState_,
               keystate_) != EOF) {
    draw();
    printf("%d\n", COMMAND_FRAMEEND);
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

int keydown_(int key) {
  return keystate_[key]-'0';
}

int keyPressed(int key) {
  if (key < 0 or key >= KEY_NUM) return false;

  if (key == KEY_SHIFT) return keydown_(KEY_LSHIFT) | keydown_(KEY_RSHIFT);
  if (key == KEY_CTRL) return keydown_(KEY_LCTRL) | keydown_(KEY_RCTRL);
  if (key == KEY_ALT) return keydown_(KEY_LALT) | keydown_(KEY_RALT);
  return keydown_(key);
}

void background(int r, int g, int b) {
  if (r < 0) r = 0; if (r > 255) r = 255;
  if (g < 0) g = 0; if (g > 255) g = 255;
  if (b < 0) b = 0; if (b > 255) b = 255;
  printf("%d %d %d %d\n", COMMAND_BACKGROUND, r, g, b);
}

void point(int x, int y) {
  printf("%d %d %d\n", COMMAND_POINT, x, y);
}

void line(int x0, int y0, int x1, int y1) {
  printf("%d %d %d %d %d\n", COMMAND_LINE, x0, y0, x1, y1);
}

void rect(int x, int y, int w, int h) {
  if (w < 0) w = 0;
  if (h < 0) h = 0;
  printf("%d %d %d %d %d\n", COMMAND_RECT, x, y, w, h);
}

void circle(int x, int y, int r) {
  if (r < 0) r = 0;
  printf("%d %d %d %d\n", COMMAND_CIRCLE, x, y, r);
}

void fill(int r, int g, int b, int a = 0xff) {
  if (r < 0) r = 0; if (r > 255) r = 255;
  if (g < 0) g = 0; if (g > 255) g = 255;
  if (b < 0) b = 0; if (b > 255) b = 255;
  if (a < 0) a = 0; if (a > 255) a = 255;
  printf("%d %d %d %d %d\n", COMMAND_FILLCOLOR, r, g, b, a);
}

void stroke(int r, int g, int b, int a = 0xff) {
  if (r < 0) r = 0; if (r > 255) r = 255;
  if (g < 0) g = 0; if (g > 255) g = 255;
  if (b < 0) b = 0; if (b > 255) b = 255;
  if (a < 0) a = 0; if (a > 255) a = 255;
  printf("%d %d %d %d %d\n", COMMAND_STROKECOLOR, r, g, b, a);
}

void strokeThickness(int thickness) {
  if (thickness < 0) thickness = 0;
  printf("%d %d\n", COMMAND_STROKETHICKNESS, thickness);
}

void text(const char* t, int x, int y) {
  printf("%d %d %d %s\n", COMMAND_TEXT, x, y, t);
}

void textSize(int s) {
  if (s < 0) s = 0;
  printf("%d %d\n", COMMAND_TEXTSIZE, s);
}

void exit() {
  running_ = 0;
}
