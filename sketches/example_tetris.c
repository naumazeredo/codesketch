#include "sketch.h"

#include <time.h>
#include <stdlib.h>

enum { DIR_X, DIR_U, DIR_D, DIR_L, DIR_R, DIR_NUM };

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

const int pieceTotalRot[] = { 1, 2, 4, 4, 1, 2, 4, 2, 1 };

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

const int BOARD_W = 10, BOARD_H = 23, BOARD_TOP_H = 3, PIECE_SIZE = 20;

int boardX, boardY;
int board[BOARD_H][BOARD_W];

int piece, pieceX, pieceY, pieceR;
int next, nextR, nextX, nextY;

int timer, difficulty;
int keys[KEY_NUM];

void drawSquare(int x, int y, int p) {
  fill(colors[p][0], colors[p][1], colors[p][2]);
  rect(x, y, PIECE_SIZE, PIECE_SIZE);
}

void createNext() {
  next = rand()%(PIECE_NUM-1)+1;
  nextR = rand()%pieceTotalRot[piece];
}

void createPiece() {
  if (!next)
    createNext();

  piece = next;
  pieceX = 3;
  pieceY = 0;

  createNext();
}

void movePiece(int dir) {
  if (dir == DIR_U) pieceY--;
  if (dir == DIR_D) pieceY++;
  if (dir == DIR_R) pieceX++;
  if (dir == DIR_L) pieceX--;
}

int canMovePiece(int dir) {
  int nX = pieceX, nY = pieceY;
  if (dir == DIR_U) nY--;
  if (dir == DIR_D) nY++;
  if (dir == DIR_R) nX++;
  if (dir == DIR_L) nX--;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      char c;
      if (pieceR == 0) c = pieces[piece][i][j];
      if (pieceR == 1) c = pieces[piece][3-j][i];
      if (pieceR == 2) c = pieces[piece][3-i][3-j];
      if (pieceR == 3) c = pieces[piece][j][3-i];
      if (c != ' ') {
        int x = nX + j, y = nY + i;
        if (x < 0 or x >= BOARD_W or y >= BOARD_H or y < 0 or board[y][x])
          return 0;
      }
    }
  }

  return 1;
}

int checkGameOver() {
  return !canMovePiece(DIR_X);
}

void dunkPiece() {
  while (canMovePiece(DIR_D)) movePiece(DIR_D);
  timer = 0;
}

int rotatePiece() {
  if (pieceTotalRot[piece] == 1) return 1;

  int rot = pieceR;

  // Try to rotate
  pieceR = (rot + 1)%pieceTotalRot[piece];
  for (int i = DIR_X; i < DIR_NUM; ++i) if (canMovePiece(i)) {
    movePiece(i);
    return 1;
  }

  // If piece is I we have some problems with rotations
  // so we treat them apart
  if (piece == PIECE_I) {
    movePiece(DIR_R);
    if (canMovePiece(DIR_R)) {
      movePiece(DIR_R);
      return 1;
    }
    movePiece(DIR_L);
  }

  pieceR = rot;

  return 0;
}

void drawPiece(int x, int y) {
  for (int i = 0; i < 4; ++i) {
    if (i + pieceY < BOARD_TOP_H) continue;

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

void removeLine(int x) {
  for (int i = x; i > 0; --i)
    for (int j = 0; j < BOARD_W; ++j)
      board[i][j] = board[i-1][j];

  for (int j = 0; j < BOARD_W; ++j)
    board[0][j] = 0;
}

void removeCompleteLines() {
  for (int i = BOARD_TOP_H; i < BOARD_H; ++i) {
    int tot = 0;
    for (int j = 0; j < BOARD_W; ++j)
      tot += !!board[i][j];
    if (tot == BOARD_W)
      removeLine(i);
  }
}

void addPieceToBoard() {
  for (int i = 0; i < 4; ++i) {
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

void drawNext() {
  fill(0, 0, 0);
  rect(nextX, nextY, 6 * PIECE_SIZE, 6 * PIECE_SIZE);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      char c;
      if (nextR == 0) c = pieces[next][i][j];
      if (nextR == 1) c = pieces[next][3-j][i];
      if (nextR == 2) c = pieces[next][3-i][3-j];
      if (nextR == 3) c = pieces[next][j][3-i];
      if (c != ' ')
        drawSquare(nextX + (j+1) * PIECE_SIZE, nextY + (i+1) * PIECE_SIZE, next);
    }
  }
}

void drawBoard() {
  for (int i = 0; i < BOARD_H - BOARD_TOP_H; ++i)
    for (int j = 0; j < BOARD_W; ++j)
      drawSquare(boardX + j * PIECE_SIZE, boardY + i * PIECE_SIZE, board[i+BOARD_TOP_H][j]);
  drawPiece(boardX, boardY - BOARD_TOP_H * PIECE_SIZE);
}

void startGame() {
  // Timer
  difficulty = 20; // number of draws until move
  timer = 0;

  // Board
  boardX = (windowWidth - PIECE_SIZE * BOARD_W) / 2;
  boardY = 20;

  for (int i = 0; i < BOARD_H; ++i)
    for (int j = 0; j < BOARD_W; ++j)
      board[i][j] = PIECE_NONE;

  // Piece
  piece = 0;

  // Next
  nextX = boardX + (BOARD_W + 1) * PIECE_SIZE;
  nextY = boardY + 5 * PIECE_SIZE;
}

void resetTimer() {
  timer = difficulty;
}

void triggerTimer() {
  // Try to move piece
  if (piece and canMovePiece(DIR_D)) {
    movePiece(DIR_D);
  } else {
    addPieceToBoard();
    removeCompleteLines();

    createPiece();

    if (checkGameOver()) {
      startGame();
    }
  }

  resetTimer();
}

int keyPressed(int key) {
  return keyDown(key) and !keys[key];
}

void updateKeys() {
  static int keylist[] = { KEY_UP, KEY_SPACE };
  for (int i = 0; i < sizeof(keylist)/sizeof(keylist[0]); ++i)
    keys[keylist[i]] = keyDown(keylist[i]);
}

void setup() {
  framerate(20);

  // Random seed
  srand(time(0));

  startGame();
}

void draw() {
  if (keyDown(KEY_LEFT)  and canMovePiece(DIR_L)) movePiece(DIR_L);
  if (keyDown(KEY_RIGHT) and canMovePiece(DIR_R)) movePiece(DIR_R);
  if (keyDown(KEY_DOWN)  and canMovePiece(DIR_D)) movePiece(DIR_D), resetTimer();
  if (keyPressed(KEY_SPACE)) dunkPiece(), triggerTimer();
  if (keyPressed(KEY_UP))    rotatePiece();

  timer--;
  if (timer < 0)
    triggerTimer();

  background(127, 127, 127);

  updateKeys();

  drawBoard();
  drawNext();
}

