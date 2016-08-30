#include "sketch.h"

#include <time.h>
#include <stdlib.h>

#define PIECE_ITER_BEGIN(piece)                             \
  for (int i = 0; i < 4; ++i) {                             \
    for (int j = 0; j < 4; ++j) {                           \
      char c;                                               \
      if (piece.rot == 0) c = pieces[piece.type][i][j];     \
      if (piece.rot == 1) c = pieces[piece.type][3-j][i];   \
      if (piece.rot == 2) c = pieces[piece.type][3-i][3-j]; \
      if (piece.rot == 3) c = pieces[piece.type][j][3-i];   \
      if (c != ' ') {                                       \

#define PIECE_ITER_END() \
      }                  \
    }                    \
  }

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

struct Piece {
  int type, x, y, rot;
} piece, next, hold;

int holdCnt;

int timer, difficulty;
int keys[KEY_NUM];

void drawSquare(int x, int y, int p) {
  fill(colors[p][0], colors[p][1], colors[p][2]);
  rect(x, y, PIECE_SIZE, PIECE_SIZE);
}

void holdPiece() {
  if (holdCnt <= 0) return;

  if (!hold.type) {
    hold.type  = piece.type;
    hold.rot   = piece.rot;
    piece.type = 0;
  } else {
    Piece tmp;
    tmp.type   = hold.type;
    tmp.rot    = hold.rot;

    hold.type  = piece.type;
    hold.rot   = piece.rot;

    piece.type = tmp.type;
    piece.rot  = tmp.rot;
  }

  holdCnt--;
}

void createNext() {
  next.type = rand()%(PIECE_NUM-1)+1;
  next.rot = rand()%pieceTotalRot[piece.type];
}

void createPiece() {
  if (!next.type)
    createNext();

  piece.type = next.type;
  piece.rot = next.rot;
  piece.x = 3;
  piece.y = 0;

  createNext();

  // Reset hold
  holdCnt = 1;
}

void movePiece(int dir) {
  if (dir == DIR_U) piece.y--;
  if (dir == DIR_D) piece.y++;
  if (dir == DIR_R) piece.x++;
  if (dir == DIR_L) piece.x--;
}

int canMovePiece(int dir) {
  int nx = piece.x, ny = piece.y;
  if (dir == DIR_U) ny--;
  if (dir == DIR_D) ny++;
  if (dir == DIR_R) nx++;
  if (dir == DIR_L) nx--;

  PIECE_ITER_BEGIN(piece)
    int x = nx + j, y = ny + i;
    if (x < 0 or x >= BOARD_W or y >= BOARD_H or y < 0 or board[y][x]) {
      return 0;
    }
  PIECE_ITER_END()

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
  if (pieceTotalRot[piece.type] == 1) return 1;

  int rot = piece.rot;

  // Try to rotate
  piece.rot = (rot + 1)%pieceTotalRot[piece.type];
  for (int i = DIR_X; i < DIR_NUM; ++i) if (canMovePiece(i)) {
    movePiece(i);
    return 1;
  }

  // If piece is I we have some problems with rotations
  // so we treat them apart
  if (piece.type == PIECE_I) {
    movePiece(DIR_R);
    if (canMovePiece(DIR_R)) {
      movePiece(DIR_R);
      return 1;
    }
    movePiece(DIR_L);
  }

  piece.rot = rot;

  return 0;
}

void drawPiece(int x, int y) {
  PIECE_ITER_BEGIN(piece)
    if (i + piece.y < BOARD_TOP_H) continue;
    drawSquare(x + (piece.x + j) * PIECE_SIZE, y + (piece.y + i) * PIECE_SIZE, piece.type);
  PIECE_ITER_END();
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
  PIECE_ITER_BEGIN(piece)
    int x = piece.x + j, y = piece.y + i;
    if (x >= 0 and x < BOARD_W and y >= 0 and y < BOARD_H)
      board[y][x] = piece.type;
  PIECE_ITER_END();
}

void drawNext() {
  fill(0, 0, 0);
  rect(next.x, next.y, 6 * PIECE_SIZE, 6 * PIECE_SIZE);

  fill(255, 255, 255);
  text(next.x +     PIECE_SIZE - 8, next.y - 8, "N");
  text(next.x + 2 * PIECE_SIZE - 8, next.y - 8, "E");
  text(next.x + 3 * PIECE_SIZE - 8, next.y - 8, "X");
  text(next.x + 4 * PIECE_SIZE - 8, next.y - 8, "T");

  PIECE_ITER_BEGIN(next)
    drawSquare(next.x + (j+1) * PIECE_SIZE, next.y + (i+1) * PIECE_SIZE, next.type);
  PIECE_ITER_END()
}

void drawHold() {
  fill(0, 0, 0);
  rect(hold.x, hold.y, 6 * PIECE_SIZE, 6 * PIECE_SIZE);

  fill(255, 255, 255);
  text(hold.x +     PIECE_SIZE - 8, hold.y - 8, "H");
  text(hold.x + 2 * PIECE_SIZE - 8, hold.y - 8, "O");
  text(hold.x + 3 * PIECE_SIZE - 8, hold.y - 8, "L");
  text(hold.x + 4 * PIECE_SIZE - 8, hold.y - 8, "D");

  PIECE_ITER_BEGIN(hold)
    drawSquare(hold.x + (j+1) * PIECE_SIZE, hold.y + (i+1) * PIECE_SIZE, hold.type);
  PIECE_ITER_END()
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
  piece.type = 0;

  // Next
  next.x = boardX + (BOARD_W + 1) * PIECE_SIZE;
  next.y = boardY + PIECE_SIZE;

  // Hold
  hold.x = boardX - 7 * PIECE_SIZE;
  hold.y = boardY + PIECE_SIZE;
}

void resetTimer() {
  timer = difficulty;
}

void triggerTimer() {
  // Try to move piece
  debug("timer pieces: %d %d", piece.type, next.type);
  if (piece.type and canMovePiece(DIR_D)) {
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
  static int keylist[] = { KEY_UP, KEY_SPACE, KEY_C };
  for (int i = 0; i < sizeof(keylist)/sizeof(keylist[0]); ++i)
    keys[keylist[i]] = keyDown(keylist[i]);
}

void setup() {
  framerate(20);
  textSize(24);

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
  if (keyPressed(KEY_C))     holdPiece();

  timer--;
  if (timer < 0)
    triggerTimer();

  background(127, 127, 127);

  updateKeys();

  drawBoard();
  drawNext();
  drawHold();
}

