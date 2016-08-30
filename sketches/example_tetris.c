#include "sketch.h"

#include <time.h>
#include <stdlib.h>

enum { DIR_U, DIR_D, DIR_L, DIR_R };

enum {
  PIECE_NONE, PIECE_I, PIECE_J, PIECE_L, PIECE_O, PIECE_S, PIECE_T, PIECE_Z,
  PIECE_NUM
};

const char pieces[][4][5] = {
  { "    ", "    ", "    ", "    " },
  { "    ", "....", "    ", "    " },
  { "    ", "... ", "  . ", "    " },
  { "    ", "... ", ".   ", "    " },
  { "    ", " .. ", " .. ", "    " },
  { "    ", " .. ", "..  ", "    " },
  { "    ", "... ", " .  ", "    " },
  { "    ", "..  ", " .. ", "    " }
};

const int colors[][3] = {
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
int keys[KEY_NUM];

void drawSquare(int x, int y, int p) {
  fill(colors[p][0], colors[p][1], colors[p][2]);
  rect(x, y, PIECE_SIZE, PIECE_SIZE);
}

void createPiece() {
  piece = rand()%(PIECE_NUM-1)+1;
  pieceX = 3;
  pieceY = -3;
  pieceR = rand()%4;
}

void movePiece(int dir) {
  if (dir == DIR_U) pieceY--;
  if (dir == DIR_D) pieceY++;
  if (dir == DIR_R) pieceX++;
  if (dir == DIR_L) pieceX--;
}

int canMovePiece(int dir) {
  int nX = pieceX, nY = pieceY;
  if (dir == DIR_D) nY++;
  if (dir == DIR_R) nX++;
  if (dir == DIR_L) nX--;

  for (int i = 0; i < 4; ++i) {
    if (i + nY < 0) continue;

    for (int j = 0; j < 4; ++j) {
      char c;
      if (pieceR == 0) c = pieces[piece][i][j];
      if (pieceR == 1) c = pieces[piece][3-j][i];
      if (pieceR == 2) c = pieces[piece][3-i][3-j];
      if (pieceR == 3) c = pieces[piece][j][3-i];
      if (c != ' ') {
        int x = nX + j, y = nY + i;
        if (x < 0 or x >= BOARD_W or y >= BOARD_H or board[y][x])
          return 0;
      }
    }
  }

  return 1;
}

void drawPiece(int x, int y) {
  for (int i = 0; i < 4; ++i) {
    if (i + pieceY < 0) continue;

    for (int j = 0; j < 4; ++j) {
      char c;
      if (pieceR == 0) c = pieces[piece][i][j];
      if (pieceR == 1) c = pieces[piece][3-j][i];
      if (pieceR == 2) c = pieces[piece][3-i][3-j];
      if (pieceR == 3) c = pieces[piece][j][3-i];
      if (c != ' ')
        drawSquare(x + (pieceX + j) * PIECE_SIZE, y + (pieceY + i) * PIECE_SIZE, piece);
    }
  }
}

void addPieceToBoard() {
  for (int i = 0; i < 4; ++i) {
    if (i + pieceY < 0) continue;

    for (int j = 0; j < 4; ++j) {
      char c;
      if (pieceR == 0) c = pieces[piece][i][j];
      if (pieceR == 1) c = pieces[piece][3-j][i];
      if (pieceR == 2) c = pieces[piece][3-i][3-j];
      if (pieceR == 3) c = pieces[piece][j][3-i];
      if (c != ' ') {
        int x = pieceX + j, y = pieceY + i;
        if (x >= 0 and x < BOARD_W and y >= 0 and y < BOARD_H)
          board[y][x] = piece;
      }
    }
  }
}

void drawBoard() {
  for (int i = 0; i < BOARD_H; ++i)
    for (int j = 0; j < BOARD_W; ++j)
      drawSquare(boardX + j * PIECE_SIZE, boardY + i * PIECE_SIZE, board[i][j]);
  drawPiece(boardX, boardY);
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
  createPiece();
}

int keyPressed(int key) {
  return keyDown(key) and !keys[key];
}

void updateKeys() {
  static int keylist[] = { KEY_LEFT, KEY_RIGHT, KEY_DOWN, KEY_UP };
  for (int i = 0; i < sizeof(keylist)/sizeof(keylist[0]); ++i)
    keys[keylist[i]] = keyDown(keylist[i]);
}

void draw() {
  timer--;
  if (timer < 0) {
    // Try to move piece
    if (canMovePiece(DIR_D)) {
      movePiece(DIR_D);
    } else {
      addPieceToBoard();
      createPiece();
    }
    timer = difficulty;
  }

  background(127, 127, 127);

  if (keyPressed(KEY_LEFT)  and canMovePiece(DIR_L)) movePiece(DIR_L);
  if (keyPressed(KEY_RIGHT) and canMovePiece(DIR_R)) movePiece(DIR_R);
  if (keyPressed(KEY_DOWN)  and canMovePiece(DIR_D)) movePiece(DIR_D);

  updateKeys();

  drawBoard();
}

