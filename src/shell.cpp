#include <sstream>

#include "shell.h"
#include "externs.h"
#include "sketch.h"
#include "text.h"

namespace codesketch {

// Variables
std::vector<std::string> shellHistory, shellOutput;
fs::path shellRoot = fs::current_path();
std::string shellInput, shellInputIncomplete;
int shellHistoryCursor = 0;

// Functions
void shellOpLs(const std::vector<std::string>& args) {
  fs::path path = ".";
  if (args.size() > 1) path = args[1];
  if (fs::is_directory(path)) {
    for (auto& p : fs::directory_iterator(path)) {
      if (fs::is_directory(p.path()))
        shellAddOutput(p.path().filename().string() + "/");
      else
        shellAddOutput(p.path().filename().string());
    }
  } else if (fs::exists(path)) {
    shellAddOutput(path.filename().string());
  } else {
    shellAddOutput("no file or directory");
  }
}

void shellOpCd(const std::vector<std::string>& args) {
  if (args.size() == 1) {
    fs::current_path(shellRoot);
  } else if (fs::is_directory(args[1]) or fs::is_directory(fs::current_path() / args[1])) {
    fs::current_path(fs::canonical(args[1]));
  } else {
    shellAddOutput("cd: no such directory");
    return;
  }

  shellAddOutput(fs::current_path().string());
}

void shellOpRun(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    shellAddOutput("run expects one argument");
    return;
  }

  fs::path run_path = fs::current_path() / args[1];
  if (fs::is_regular_file(run_path)) sketchOpen(run_path.string());
  else shellAddOutput("run: no such file\n");
}

void shellOpHelp(const std::vector<std::string>& args) {
  shellAddOutput(
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
    shellAddOutput("What manual page do you want?");
    return;
  }

  if (args[1] == "ls")   shellAddOutput("list directory contents\nusage: ls");
  if (args[1] == "cd")   shellAddOutput("change directory\nusage: cd <directory>");
  if (args[1] == "run")  shellAddOutput("run sketch\nusage: run <sketch>");
  if (args[1] == "help") shellAddOutput("srsly? just type it!\nusage: help");
  if (args[1] == "man")  shellAddOutput("u\'re using it right now...\nusage: man <command>");
  if (args[1] == "exit") shellAddOutput("self explanatory, isn't it?\nusage: exit");
}

void shellOpExit(const std::vector<std::string>& args) {
  window.close();
}

void shellAddOutput(const std::string& text) {
  std::istringstream iss(text);
  std::string token;
  while (getline(iss, token, '\n')) {
    // TODO(naum): split text if texture bigger than shell size
    shellOutput.push_back(token);
  }

  int outputExtra = shellOutput.size() - shellH / shellLineH+1;
  if (outputExtra > 0) {
    shellOutput.erase(shellOutput.begin(),
                      shellOutput.begin() + outputExtra);
  }
}

void shellClearOutput() {
  shellOutput.clear();
}

void shellParseInput() {
  shellHistory.push_back(shellInput);
  shellHistoryCursor = shellHistory.size();
  shellInputIncomplete.clear();

  std::vector<std::string> args;
  std::istringstream iss(shellInput);
  std::string token;
  while (getline(iss, token, ' '))
    args.push_back(token);

  shellAddOutput(shellPS1 + shellInput);
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
    shellAddOutput("command not found.");
}

void shellDraw() {
  textRender("Code Sketch", 20, 8, 32, sf::Color::White);

  for (int i = 0; i < (int)shellOutput.size(); ++i) {
    std::string& output = shellOutput[i];
    if (output.length())
      textRender(output,
                 shellX, shellY + i * shellLineH,
                 shellTextSize, sf::Color::White);
  }

  textRender(shellPS1 + shellInput,
             shellX, shellY + shellOutput.size() * shellLineH,
             shellTextSize, sf::Color::White);
}

void shellBackspace() {
  if (shellInput.length())
    shellInput.pop_back();
}

void shellAddInput(u32 unicode) {
  // Filter unicode to ascii
  if (unicode < 32 or unicode >= 127) return;
  shellInput += unicode;
}

void shellMoveCursorUp() {
  if (shellHistoryCursor == (int)shellHistory.size())
    shellInputIncomplete = shellInput;

  if (shellHistoryCursor > 0) {
    shellHistoryCursor--;
    shellInput = shellHistory[shellHistoryCursor];
  }
}

void shellMoveCursorDown() {
  if (shellHistoryCursor < (int)shellHistory.size()) {
    shellHistoryCursor++;

    if (shellHistoryCursor == (int)shellHistory.size())
      shellInput = shellInputIncomplete;
    else
      shellInput = shellHistory[shellHistoryCursor];
  }
}

}
