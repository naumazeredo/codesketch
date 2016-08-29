#include <stdio.h>
#include <stdint.h>

enum {
  _COMMAND_FRAMEEND,
  _COMMAND_DEBUG,

  _COMMAND_BACKGROUND,

  _COMMAND_POINT,
  _COMMAND_LINE,
  _COMMAND_RECT,
  _COMMAND_CIRCLE,

  _COMMAND_FILLCOLOR,

  _COMMAND_STROKECOLOR,
  _COMMAND_STROKETHICKNESS,

  _COMMAND_TEXT,
  _COMMAND_TEXTSIZE,

  _COMMAND_CAMERA,

  // XXX(naum): Not implemented
  _COMMAND_ELLIPSE,
  _COMMAND_WINDOW,
  //

  _COMMAND_NUM
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

enum {
  MOUSE_LEFT,
  MOUSE_RIGHT,
  MOUSE_MIDDLE,
  MOUSE_X1,
  MOUSE_X2,

  MOUSE_NUM
};

int frameCount;
int windowWidth, windowHeight;
int mouseX, mouseY;

int _mouseState_;
char _keystate_[256];
int _running_;

// User declared functions
void setup();
void draw();

int main() {
  // Unbuffered stdout
  setbuf(stdout, 0);

  _running_ = 1;

  // Setup
  scanf("%d %d %d %d %d %d %s",
        &frameCount,
        &windowWidth, &windowHeight,
        &mouseX, &mouseY, &_mouseState_,
        _keystate_);
  setup();
  printf("%d\n", _COMMAND_FRAMEEND);

  while (_running_ &&
         scanf("%d %d %d %d %d %d %s",
               &frameCount,
               &windowWidth, &windowHeight,
               &mouseX, &mouseY, &_mouseState_,
               _keystate_) != EOF) {
    draw();
    printf("%d\n", _COMMAND_FRAMEEND);
  }

  return 0;
}

// Mouse and keyboard
int mouseDown(int button) {
  if (button >= MOUSE_NUM or button < 0)
    return 0;
  return _mouseState_ & (1<<button);
}

int _keydown_(int key) {
  return _keystate_[key]-'0';
}

int keyDown(int key) {
  if (key < 0 or key >= KEY_NUM) return false;

  if (key == KEY_SHIFT) return _keydown_(KEY_LSHIFT) | _keydown_(KEY_RSHIFT);
  if (key == KEY_CTRL) return _keydown_(KEY_LCTRL) | _keydown_(KEY_RCTRL);
  if (key == KEY_ALT) return _keydown_(KEY_LALT) | _keydown_(KEY_RALT);
  return _keydown_(key);
}

void background(int r, int g, int b) {
  if (r < 0) r = 0; if (r > 255) r = 255;
  if (g < 0) g = 0; if (g > 255) g = 255;
  if (b < 0) b = 0; if (b > 255) b = 255;
  printf("%d %d %d %d\n", _COMMAND_BACKGROUND, r, g, b);
}

void point(int x, int y) {
  printf("%d %d %d\n", _COMMAND_POINT, x, y);
}

void line(int x0, int y0, int x1, int y1) {
  printf("%d %d %d %d %d\n", _COMMAND_LINE, x0, y0, x1, y1);
}

void rect(int x, int y, int w, int h) {
  if (w < 0) w = 0;
  if (h < 0) h = 0;
  printf("%d %d %d %d %d\n", _COMMAND_RECT, x, y, w, h);
}

void circle(int x, int y, int r) {
  if (r < 0) r = 0;
  printf("%d %d %d %d\n", _COMMAND_CIRCLE, x, y, r);
}

void fill(int r, int g, int b, int a = 0xff) {
  if (r < 0) r = 0; if (r > 255) r = 255;
  if (g < 0) g = 0; if (g > 255) g = 255;
  if (b < 0) b = 0; if (b > 255) b = 255;
  if (a < 0) a = 0; if (a > 255) a = 255;
  printf("%d %d %d %d %d\n", _COMMAND_FILLCOLOR, r, g, b, a);
}

void stroke(int r, int g, int b, int a = 0xff) {
  if (r < 0) r = 0; if (r > 255) r = 255;
  if (g < 0) g = 0; if (g > 255) g = 255;
  if (b < 0) b = 0; if (b > 255) b = 255;
  if (a < 0) a = 0; if (a > 255) a = 255;
  printf("%d %d %d %d %d\n", _COMMAND_STROKECOLOR, r, g, b, a);
}

void strokeThickness(int thickness) {
  if (thickness < 0) thickness = 0;
  printf("%d %d\n", _COMMAND_STROKETHICKNESS, thickness);
}

void text(const char* t, int x, int y) {
  printf("%d %d %d %s\n", _COMMAND_TEXT, x, y, t);
}

void textSize(int s) {
  if (s < 0) s = 0;
  printf("%d %d\n", _COMMAND_TEXTSIZE, s);
}

void camera(int x, int y) {
  printf("%d %d %d\n", _COMMAND_CAMERA, x, y);
}

void debug(const char* s) {
  printf("%d %s\n", _COMMAND_DEBUG, s);
}

void exit() {
  _running_ = 0;
}
