#include "sketch.h"

#include <time.h>
#include <stdlib.h>

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
    "    ",
    "    ",
    "    " },
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

const int BOARD_W = 10, BOARD_H = 20, PIECE_SIZE = 20;

int boardX, boardY;
int board[BOARD_H][BOARD_W];

int piece, pieceX, pieceY, pieceR;

int timer, difficulty;

void drawSquare(int x, int y, int p) {
  fill(color[p][0], color[p][1], color[p][2]);
  rect(x, y, PIECE_SIZE, PIECE_SIZE);
}

void newPiece() {
  piece = rand()%(PIECE_NUM-1)+1;
  pieceX = 3;
  pieceY = -2;
  //pieceR = rand()%4;
  pieceR = 0;

  debug("piece: %d\n", piece);
}

void drawPiece() {
  for (int i = 0; i < 4; ++i) {
    if (i + pieceY < 0) continue;

    for (int j = 0; j < 4; ++j) {
      char c;
      if (pieceR == 0) c = pieces[piece][i][j];
      if (pieceR == 1) c = pieces[piece][3-j][i];
      if (pieceR == 2) c = pieces[piece][3-i][3-j];
      if (pieceR == 3) c = pieces[piece][j][3-i];
      if (c != ' ')
        drawSquare(boardX + (pieceX + j) * PIECE_SIZE, boardY + (pieceY + i) * PIECE_SIZE, piece);
    }
  }
}

void drawBoard() {
  for (int i = 0; i < BOARD_H; ++i)
    for (int j = 0; j < BOARD_W; ++j)
      drawSquare(boardX + j * PIECE_SIZE, boardY + i * PIECE_SIZE, board[i][j]);
  drawPiece();
}

void setup() {
  // Random seed
  srand(time(0));

  // Timer
  difficulty = 60; // number of draws until move
  timer = difficulty;

  // Board
  boardX = (windowWidth - PIECE_SIZE * BOARD_W) / 2;
  boardY = 20;

  for (int i = 0; i < BOARD_H; ++i)
    for (int j = 0; j < BOARD_W; ++j)
      board[i][j] = PIECE_NONE;

  // Piece
  newPiece();
}

void draw() {
  timer--;
  if (timer < 0) {
    // Try to move piece
    pieceY++;
    timer = difficulty;
  }

  background(127, 127, 127);
  drawBoard();
}

