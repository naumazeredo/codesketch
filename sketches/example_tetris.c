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
  PIECE_I, PIECE_J, PIECE_L, PIECE_O, PIECE_S, PIECE_T, PIECE_Z,
  PIECE_NONE, PIECE_NUM
};

const char pieces[][4][5] = {
  { "    ", "....", "    ", "    " },
  { "    ", "... ", "  . ", "    " },
  { "    ", " ...", " .  ", "    " },
  { "    ", " .. ", " .. ", "    " },
  { "    ", " .. ", "..  ", "    " },
  { "    ", " .  ", "... ", "    " },
  { "    ", "..  ", " .. ", "    " }
};

const int pieceTotalRot[] = { 2, 4, 4, 1, 2, 4, 2 };

const int colors[][3] = {
  {0x00, 0xff, 0xff},
  {0x00, 0x00, 0xff},
  {0xff, 0xa5, 0x00},
  {0xff, 0xff, 0x00},
  {0x80, 0xff, 0x00},
  {0x80, 0x00, 0x80},
  {0xff, 0x00, 0x00},
  {0x00, 0x00, 0x00}
};

const int difftable[] = {
  53, 49, 45, 41, 37, 33, 28, 22, 17, 11, 10,
  9, 8, 7, 6, 6, 5, 5, 4, 4, 3
};

const int BOARD_W = 10, BOARD_H = 23, BOARD_TOP_H = 3, PIECE_SIZE = 20;

int boardX, boardY;
int board[BOARD_H][BOARD_W];

struct Piece {
  int type, x, y, rot;
} piece, next, hold;
int previousPiece;

int holdCnt;

int level, levelIncrease, score;
int lines;

int timer, difficulty;
int keys[KEY_NUM];

int playing, playingTimer;

void createNext();
void createPiece();

int canMovePiece(int);
void movePiece(int);
void holdPiece();
void dunkPiece();
int rotatePiece();

void scoreLines(int);
void removeLine(int);
void removeCompleteLines();
void addPieceToBoard();

void drawSquare(int, int, int);
void drawPiece(int, int);
void drawNext();
void drawHold();
void drawBoard();
void drawLevel();
void drawLines();
void drawScore();

void startGame();
int checkGameOver();

void resetTimer();
void triggerTimer();
int keyPressed();
void updateKeys();

void createNext() {
  next.type = rand()%7;

  // Rerandomize in case of repeated piece
  // to avoid equals
  if (next.type == previousPiece)
    next.type = rand()%7;

  next.rot = 0;
}

void createPiece() {
  previousPiece = piece.type;

  if (next.type == PIECE_NONE)
    createNext();

  piece.type = next.type;
  piece.rot = next.rot;
  piece.x = 3;
  piece.y = 1;

  createNext();

  // Reset hold
  holdCnt = 1;
}

int canMovePiece(int dir) {
  int nx = piece.x, ny = piece.y;
  if (dir == DIR_U) ny--;
  if (dir == DIR_D) ny++;
  if (dir == DIR_R) nx++;
  if (dir == DIR_L) nx--;

  PIECE_ITER_BEGIN(piece)
    int x = nx + j, y = ny + i;
    if (x < 0 or x >= BOARD_W or y >= BOARD_H or y < 0 or board[y][x] != PIECE_NONE) {
      return 0;
    }
  PIECE_ITER_END()

  return 1;
}

void movePiece(int dir) {
  if (dir == DIR_U) piece.y--;
  if (dir == DIR_D) piece.y++;
  if (dir == DIR_R) piece.x++;
  if (dir == DIR_L) piece.x--;
}

void holdPiece() {
  if (holdCnt <= 0) return;

  Piece tmp;
  tmp.type   = piece.type;
  tmp.rot    = piece.rot;

  piece.type = hold.type;
  piece.rot  = 0;

  if (piece.type == PIECE_NONE) {
    hold.type  = tmp.type;
    holdCnt--;

    triggerTimer();
  } else {
    for (int i = DIR_X; i < DIR_NUM; ++i) if (canMovePiece(i)) {
      movePiece(i);
      hold.type  = tmp.type;
      holdCnt--;

      piece.x = 3;
      piece.y = 1;
      return;
    }

    piece.type = tmp.type;
    piece.rot  = tmp.rot;
  }
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

void scoreLines(int n) {
  static int scoretable[] = { 0, 40, 100, 300, 1200 };
  score += scoretable[n] * (level + 1);

  lines += n;

  if (level < 20) {
    levelIncrease -= n;
    if (levelIncrease <= 0) {
      level++;
      difficulty = difftable[level];
      levelIncrease += 10;
    }
  }
}

void removeLine(int x) {
  for (int i = x; i > 0; --i)
    for (int j = 0; j < BOARD_W; ++j)
      board[i][j] = board[i-1][j];

  for (int j = 0; j < BOARD_W; ++j)
    board[0][j] = PIECE_NONE;
}

void removeCompleteLines() {
  int cnt = 0;

  for (int i = BOARD_TOP_H; i < BOARD_H; ++i) {
    int tot = 0;
    for (int j = 0; j < BOARD_W; ++j)
      tot += board[i][j]!=PIECE_NONE;
    if (tot == BOARD_W)
      removeLine(i), cnt++;
  }

  scoreLines(cnt);
}

void addPieceToBoard() {
  if (piece.type == PIECE_NONE) return;

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

void drawSquare(int x, int y, int p) {
  fill(colors[p][0], colors[p][1], colors[p][2]);
  rect(x, y, PIECE_SIZE, PIECE_SIZE);
}

void drawPiece(int x, int y) {
  PIECE_ITER_BEGIN(piece)
    if (i + piece.y < BOARD_TOP_H) continue;
    drawSquare(x + (piece.x + j) * PIECE_SIZE, y + (piece.y + i) * PIECE_SIZE, piece.type);
  PIECE_ITER_END();
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

void drawLevel() {
  fill(255, 255, 255);
  int x = boardX + (BOARD_W + 1) * PIECE_SIZE,
      y = boardY + 8 * PIECE_SIZE;
  text(x, y, "LEVEL: %2d", level);
}

void drawLines() {
  fill(255, 255, 255);
  int x = boardX + (BOARD_W + 1) * PIECE_SIZE,
      y = boardY + 10 * PIECE_SIZE;
  text(x, y, "LINES: %2d", lines);
}

void drawScore() {
  fill(255, 255, 255);
  int x = boardX, y = boardY + (BOARD_H - BOARD_TOP_H) * PIECE_SIZE;
  text(x, y, "SCORE: %8d", score);
}

void drawControls() {
  fill(255, 255, 255);
  int x = boardX - 7 * PIECE_SIZE, y = boardY + 8 * PIECE_SIZE;
  text(x, y   , "CONTROLS");
  text(x, y+ 40, "MOVE:ARR");
  text(x, y+ 60, "HOLD:  C");
  text(x, y+ 80, "DROP:SPC");
  text(x, y+100, "QUIT:ESC");
}

void resetGame() {
  // Level and score
  level = 0;
  score = 0;
  levelIncrease = 10;
  lines = 0;

  // Timer
  difficulty = difftable[0];
  timer = 0;

  // Board
  boardX = (windowWidth - PIECE_SIZE * BOARD_W) / 2;
  boardY = 20;

  for (int i = 0; i < BOARD_H; ++i)
    for (int j = 0; j < BOARD_W; ++j)
      board[i][j] = PIECE_NONE;

  // Piece
  piece.type = PIECE_NONE;

  // Next
  next.type = PIECE_NONE;
  next.x = boardX + (BOARD_W + 1) * PIECE_SIZE;
  next.y = boardY + PIECE_SIZE;

  // Hold
  hold.type = PIECE_NONE;
  hold.x = boardX - 7 * PIECE_SIZE;
  hold.y = boardY + PIECE_SIZE;
}

void endGame() {
  playing = 0;
  playingTimer = 30;
}

void startGame() {
  playing = 1;
  resetGame();
}

int checkGameOver() {
  return !canMovePiece(DIR_X);
}

void resetTimer() {
  timer = difficulty;
}

void triggerTimer() {
  // Try to move piece
  if (piece.type != PIECE_NONE and canMovePiece(DIR_D)) {
    movePiece(DIR_D);
  } else {
    addPieceToBoard();
    removeCompleteLines();

    createPiece();

    if (checkGameOver())
      endGame();
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
  textSize(24);

  // Random seed
  srand(time(0));

  resetGame();
  endGame();
}

void draw() {
  if (playing) {
    if (frameCount%3 == 0) {
      if (keyDown(KEY_LEFT)  and canMovePiece(DIR_L)) movePiece(DIR_L);
      if (keyDown(KEY_RIGHT) and canMovePiece(DIR_R)) movePiece(DIR_R);
      if (keyDown(KEY_DOWN)  and canMovePiece(DIR_D)) movePiece(DIR_D), resetTimer();
    }

    if (keyPressed(KEY_SPACE)) dunkPiece(), triggerTimer();
    if (keyPressed(KEY_UP))    rotatePiece();
    if (keyPressed(KEY_C))     holdPiece();

    timer--;
    if (timer < 0)
      triggerTimer();
  } else {
    playingTimer--;
    if (playingTimer <= 0) {
      if (keyDown(KEY_SPACE))
        startGame();
    }
  }

  updateKeys();
  background(127, 127, 127);

  drawBoard();
  drawNext();
  drawHold();
  drawScore();
  drawLevel();
  drawLines();
  drawControls();

  if (!playing) {
    fill(0, 0, 0);
    int y = windowHeight / 2 - 20;
    rect(windowWidth / 2 - 120, y, 250, 60);

    fill(255, 255, 255);
    text(windowWidth / 2 - 40, y, "TETRIS");
    if (playingTimer <= 0)
      text(windowWidth / 2 - 120, y + 20, "Press SPACE to start!");
  }
}

