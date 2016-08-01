#include <cstdio>

enum Commands {
  NOTHING,
  BACKGROUND,
  COLOR,
  POINT,
  LINE,
  RECT,
  NUM
};

int main() {
  setbuf(stdout, 0);

  int wW, wH, mX, mY, mS;
  char keys[1024];

  while (~scanf("%d %d %d %d %d %s", &wW, &wH, &mX, &mY, &mS, keys)) {
    printf("%c %d %d %d\n", Commands::BACKGROUND, 128, 128, 128);
    printf("%c %d %d %d\n", Commands::COLOR, 0, 0, 0);
    for (int i = 0; i <= 480; i+=9)
      printf("%c %d %d %d %d\n", Commands::LINE, i*4/3, 0, 640, i);
    printf("%c %d %d %d %d", Commands::RECT, 100, 100, 50, 50);
  }

  return 0;
}
