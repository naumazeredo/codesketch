#include "shell.h"

#include <cstdio>

#include <experimental/filesystem>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <utility>

#include "externs.h"
#include "sketch.h"
#include "text.h"

namespace codesketch {

// Variables
std::vector<std::string> shellHistory;
fs::path shellRoot = fs::current_path();
std::string shellInput;

// Functions
void shellOpLs(const std::vector<std::string>& args) {
  fs::path path = ".";
  if (args.size() > 1) path = args[1];
  if (fs::is_directory(path)) {
    for (auto& p : fs::directory_iterator(path)) {
      if (fs::is_directory(p.path()))
        shellAddHistory(p.path().filename().string() + "/");
      else
        shellAddHistory(p.path().filename().string());
    }
  } else if (fs::exists(path)) {
    shellAddHistory(path.filename().string());
  } else {
    shellAddHistory("no file or directory");
  }
}

void shellOpCd(const std::vector<std::string>& args) {
  if (args.size() == 1) {
    fs::current_path(shellRoot);
  } else if (fs::is_directory(fs::current_path() / args[1])) {
    fs::current_path(fs::current_path() / args[1]);
  } else {
    shellAddHistory("cd: no such directory");
    return;
  }

  shellAddHistory(fs::current_path().string());
}

void shellOpRun(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    shellAddHistory("run expects one argument");
    return;
  }

  fs::path run_path = fs::current_path() / args[1];
  // FIXME(naum): Uncomment when sketch file is SFML-safe
  /*
  if (fs::is_regular_file(run_path)) sketchOpen(run_path);
  else shellAddHistory("run: no such file\n");
  */
}

void shellOpHelp(const std::vector<std::string>& args) {
  shellAddHistory(
    "\n"
    "Code Sketch help\n"
    "\n"
    "commands:\n"
    "help : this page\n"
    "ls   : list directories\n"
    "cd   : change directory\n"
    "run  : run sketch\n"
    "man  : manual\n"
    "exit : exit codesketch\n"
    "\n"
    "type man <command> to see how the command works.\n"
    "\n"
    "by kogyblack"
    "\n"
  );
}

void shellOpMan(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    shellAddHistory("What manual page do you want?");
    return;
  }

  if (args[1] == "ls")   shellAddHistory("list directory contents\nusage: ls");
  if (args[1] == "cd")   shellAddHistory("change directory\nusage: cd <directory>");
  if (args[1] == "run")  shellAddHistory("run sketch\nusage: run <sketch>");
  if (args[1] == "help") shellAddHistory("srsly? just type it!\nusage: help");
  if (args[1] == "man")  shellAddHistory("u\'re using it right now...\nusage: man <command>");
  if (args[1] == "exit") shellAddHistory("self explanatory, isn't it?\nusage: exit");
}

void shellOpExit(const std::vector<std::string>& args) {
  window.close();
}

void shellAddHistory(const std::string& text) {
  std::istringstream iss(text);
  std::string token;
  while (getline(iss, token, '\n')) {
    // TODO(naum): split text if texture bigger than shell size
    shellHistory.push_back(token);
  }

  int historyExtra = shellHistory.size() - shellH / shellLineH+1;
  if (historyExtra > 0) {
    shellHistory.erase(shellHistory.begin(),
                       shellHistory.begin() + historyExtra);
  }
}

void shellClearHistory() {
  shellHistory.clear();
}

void shellParseInput() {
  std::vector<std::string> args;
  std::istringstream iss(shellInput);
  std::string token;
  while (getline(iss, token, ' '))
    args.push_back(token);

  shellAddHistory(shellPS1 + shellInput);
  shellInput.clear();

  if (args.size() > 0) {
    for (auto op : shellOps) {
      if (args[0] == op.first) {
        op.second(args);
        return;
      }
    }
  }

  if (args.size())
    shellAddHistory("command not found.");
}

void shellDraw() {
  textRender("Code Sketch", 20, 20, 32, sf::Color::White);

  for (int i = 0; i < (int)shellHistory.size(); ++i) {
    std::string& history = shellHistory[i];
    if (history.length())
      textRender(history, shellX, shellY + i * shellLineH, shellTextSize, sf::Color::White);
  }

  // TODO(naum): "Bye" with SFML
  textRender(shellPS1 + shellInput,
             shellX, shellY + shellHistory.size() * shellLineH,
             shellTextSize, sf::Color::White);
  /*
  if (window.isOpen())
    textRender(shellPS1 + shellInput,
               shellX, shellY + shellHistory.size() * shellLineH,
               shellTextSize, sf::Color::White);
  else
    textRender("Bye xD",
               shellX, shellY + shellHistory.size() * shellLineH,
               shellTextSize, sf::Color::White);
  */
}

void shellBackspace() {
  if (shellInput.length())
    shellInput.pop_back();
}

void shellAddInput(const std::string& text) {
  shellInput += text;
}

}
