#include "filesystem.h"

#include <cstdio>

#include <experimental/filesystem>
#include <array>
#include <queue>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <utility>

#include "externs.h"
#include "sketch.h"

namespace codesketch {

std::queue<std::string> shellHistory;
fs::path shellRoot = fs::current_path();

void shellOpLs(const std::vector<std::string>& args) {
  for (auto& p : fs::directory_iterator("."))
    printf("%s\n", p.path().c_str());
}

void shellOpCd(const std::vector<std::string>& args) {
  if (args.size() == 1) {
    fs::current_path(shellRoot);
  } else if (fs::is_directory(fs::current_path() / args[1])) {
    fs::current_path(fs::current_path() / args[1]);
  } else {
    printf("cd: no such directory\n");
    return;
  }

  printf("cd: %s\n", fs::current_path().c_str());
}

void shellOpRun(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    printf("run expects one argument. Type \"man run\" for help.\n");
    return;
  }

  fs::path run_path = fs::current_path() / args[1];
  if (fs::is_regular_file(run_path)) sketchOpen(run_path);
  else printf("run: no such file\n");
}

void shellOpHelp(const std::vector<std::string>& args) {
  printf("help: not implemented\n");
}

void shellOpMan(const std::vector<std::string>& args) {
  printf("man: not implemented\n");
}

void shellOpExit(const std::vector<std::string>& args) {
  printf("exit: not implemented\n");
}

void shellAddHistory(const std::string& text) {
  std::istringstream iss(text);
  std::string token;
  while (getline(iss, token, '\n')) {
    // TODO(naum)
  }
}

void shellParse(const std::string& command) {
  std::vector<std::string> args;
  std::istringstream iss(command);
  std::string token;
  while (getline(iss, token, ' '))
    args.push_back(token);

  if (args.size() > 0) {
    for (auto op : shellOps) {
      if (args[0] == op.first) {
        op.second(args);
        return;
      }
    }
  }

  printf("command not found\n");
}

}
