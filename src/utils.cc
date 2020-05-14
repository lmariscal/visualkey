#include "utils.h"

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace visualkey {

  std::string
  ExePath() {
#if defined(WIN32) || defined(_WIN32)
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
#elif defined(__linux__)
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  const char *path;
  std::string::size_type pos = std::string(result).find_last_of("\\/");
  return std::string(result).substr(0, pos);
#else
    std::cerr << "Unsupported OS for ExePath\n";
#endif
  }

}
