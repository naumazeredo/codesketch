#include <cstdio>

#include <experimental/filesystem>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <queue>
using namespace std;
using namespace std::experimental::filesystem;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "externs.h"
#include "text.h"
#include "sketch.h"

namespace codesketch {

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int windowWidth = 640, windowHeight = 480;
bool isRunning = true;

int mouseX, mouseY;
Uint32 mouseState;

// Filesystem
const int terminal_x = 20,
          terminal_y = 60,
          terminal_w = windowWidth - 40,
          terminal_h = windowHeight - 80;
const string terminal_ps1 = "> ", terminal_ps2 = "- ";
queue<string> terminal_history;
path rootPath = current_path();

void terminal_add_history(const string& text) {
  istringstream iss(text);
  string token;
  while (getline(iss, token, '\n')) {
    // TODO(naum)
  }
}

void shell_ls(const vector<string>&);
void shell_cd(const vector<string>&);
void shell_run(const vector<string>&);
void shell_help(const vector<string>&);
void shell_man(const vector<string>&);
void shell_exit(const vector<string>&);

void shell_ls(const vector<string>& args) {
  for (auto& p : directory_iterator("."))
    printf("%s\n", p.path().c_str());
}

void shell_cd(const vector<string>& args) {
  if (args.size() == 1) {
    current_path(rootPath);
    return;
  }

  path cd_path = current_path() / args[1];
  if (is_directory(cd_path)) current_path(cd_path);
  else printf("cd: no such directory\n");
}

void shell_run(const vector<string>& args) {
  if (args.size() < 2) {
    printf("run expects one argument. Type \"man run\" for help.\n");
    return;
  }

  path run_path = current_path() / args[1];
  if (is_regular_file(run_path)) sketchOpen(run_path);
  else printf("run: no such file\n");
}

void shell_help(const vector<string>& args) {
  printf("help: not implemented\n");
}

void shell_man(const vector<string>& args) {
  printf("man: not implemented\n");
}

void shell_exit(const vector<string>& args) {
  printf("exit: not implemented\n");
}

const int shellnum = 6;
string shellops[] = { "cd", "ls", "run", "help", "man", "exit" };
function<void (const vector<string>&)> shellfns[] = {
  &shell_cd, &shell_ls, &shell_run, &shell_help, &shell_man, &shell_exit
};

void shell_parse(const string& command) {
  printf("Parsing: %s\n", command.c_str());
  vector<string> args;
  istringstream iss(command);
  string token;
  while (getline(iss, token, ' '))
    args.push_back(token);

  terminal_history.push(command);

  if (args.size() > 0) {
    for (int i = 0; i < shellnum; ++i) {
      if (args[0] == shellops[i]) {
        shellfns[i](args);
        return;
      }
    }
  }

  printf("command not found\n");
}

void init() {
  // TODO(naum): treat errors
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);

  textInit();

  // Create window
  window = SDL_CreateWindow(
    "Code Sketch",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    windowWidth, windowHeight, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void shutdown() {
  textQuit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void run() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_StartTextInput();
  string textInput;

  unsigned ticks = SDL_GetTicks();
  isRunning = true;
  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        isRunning = false;
      }

      if (sketchIsRunning()) {
        if (event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_ESCAPE) {
          sketchClose();
        }
      } else {
        if (event.type == SDL_KEYDOWN) {
          if (event.key.keysym.sym == SDLK_BACKSPACE and textInput.length() > 0) {
            textInput.pop_back();
          } else if (event.key.keysym.sym == SDLK_RETURN) {
            shell_parse(textInput);
            textInput.clear();
          }
        } else if (event.type == SDL_TEXTINPUT) {
          textInput += event.text.text;
        }
      }
    }

    keystate = SDL_GetKeyboardState(nullptr);
    mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    // If loaded sketch
    if (sketchIsRunning()) {
      sketchSendData();
      sketchReceiveData();
    } else {
      SDL_RenderClear(renderer);

      textRender("Code Sketch", 20, 20, 32);
      if (textInput.length() > 0) textRender(textInput.c_str(), 20, 60);
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(max(1000/60 - (SDL_GetTicks() - ticks), (Uint32)0));
    ticks = SDL_GetTicks();
  }
}

}

int main(int argc, char** argv) {
  codesketch::init();
  codesketch::run();
  codesketch::shutdown();
}
