#ifndef CODESKETCH_CORE_SHELL_H
#define CODESKETCH_CORE_SHELL_H

#include <array>
#include <string>
#include <vector>
#include <functional>
#include <utility>

#include "externs.h"

typedef std::function<void (const std::vector<std::string>&)> ShellFn;

namespace codesketch {

// XXX(naum): remove const and do better?
const int shellTextSize = 24,
          shellLineH = shellTextSize - 4,
          shellX = 20,
          shellY = 48,
          shellW = windowWidth - 2 * shellX,
          shellH = windowHeight - shellY - shellLineH;
const std::string shellPS1 = "> ", shellPS2 = "- ";

void shellOpLs  (const std::vector<std::string>&);
void shellOpCd  (const std::vector<std::string>&);
void shellOpRun (const std::vector<std::string>&);
void shellOpHelp(const std::vector<std::string>&);
void shellOpMan (const std::vector<std::string>&);
void shellOpExit(const std::vector<std::string>&);

const std::pair<std::string, ShellFn> shellOps[] = {
  { "cd",   &shellOpCd   },
  { "ls",   &shellOpLs   },
  { "run",  &shellOpRun  },
  { "help", &shellOpHelp },
  { "man",  &shellOpMan  },
  { "exit", &shellOpExit }
};
const int shellOpsSize = sizeof(shellOps)/sizeof(shellOps[0]);

void shellAddOutput(const std::string&);
void shellClearOutput();

void shellParseInput();
void shellDraw();

void shellBackspace();
void shellAddInput(u32);

void shellMoveCursorUp();
void shellMoveCursorDown();

}

#endif
