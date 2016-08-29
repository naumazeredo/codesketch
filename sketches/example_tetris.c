#include "sketch.h"

enum {
  PIECE_NONE,
  PIECE_I, PIECE_J,
  PIECE_L, PIECE_O,
  PIECE_S, PIECE_T,
  PIECE_Z,
  PIECE_NUM
};

const char pieces[][4][5] = {
  { "    ",
    "....",
    "    ",
    "    " },
  { "    ",
    "... ",
    "  . ",
    "    " },
  { "    ",
    "... ",
    ".   ",
    "    " },
  { "    ",
    " .. ",
    " .. ",
    "    " },
  { "    ",
    " .. ",
    "..  ",
    "    " },
  { "    ",
    "... ",
    " .  ",
    "    " },
  { "    ",
    "..  ",
    " .. ",
    "    " }
};

const int color[][3] = {
  {0x00, 0x00, 0x00},
  {0x00, 0xff, 0xff},
  {0x00, 0x00, 0xff},
  {0xff, 0xa5, 0x00},
  {0xff, 0xff, 0x00},
  {0x80, 0xff, 0x00},
  {0x80, 0x00, 0x80},
  {0xff, 0x00, 0x00}
};

const int BOARD_W = 10, BOARD_H = 20,
          BOARD_PIECE_SIZE = 20;

int board[BOARD_H][BOARD_W];

void drawboard(int x, int y) {
  for (int i = 0; i < BOARD_H; ++i) {
    for (int j = 0; j < BOARD_W; ++j) {
      int piece = board[i][j];
      fill(color[piece][0], color[piece][1], color[piece][2]);
      rect(x + i     * BOARD_PIECE_SIZE, y + j     * BOARD_PIECE_SIZE,
           x + (i+1) * BOARD_PIECE_SIZE, y + (j+1) * BOARD_PIECE_SIZE);
    }
  }
}

void setup() {
  textSize(20);

  for (int i = 0; i < BOARD_H; ++i) for (int j = 0; j < BOARD_W; ++j)
    board[i][j] = (i * BOARD_W + j)%PIECE_NUM;
}

void draw() {
  background(255, 0, 0);
  drawboard(20, 20);
}

