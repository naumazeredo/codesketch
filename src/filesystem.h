#ifndef CODESKETCH_CORE_FILESYSTEM_H
#define CODESKETCH_CORE_FILESYSTEM_H

#include <cstdlib>
#include <string>

#ifdef WINDOWS
/* */
#else
#include <unistd.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

namespace fs {

class path {
public:
  path() = default;
  path(std::string s) : path_ {s} {}

  path& operator=(std::string s) {
    path_ = s;
    return this;
  }

  path& operator/(std::string s) {
    path_ += s;
    return this;
  }

  std::string string() const { return path_; }

  path filename() const {
    // Get only the filename from the path
    const auto pos = path_.rfind('/');
    if (pos != std::string::npos)
      return path_.substr(pos+1);
    return path_;
  }

private:
  std::string path_;
};

path current_path() {
  /* */
  return path();
}

path current_path(const path& p) {
}

bool is_directory(path p) {
  /* */
  return true;
}

bool exists(path p) {
  /* */
  return true;
}

}

#endif
