#include "utils.h"

#include <algorithm>
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace visualkey {

  std::string
  DistPath() {
#if defined(WIN32) || defined(_WIN32)
    char result_char[MAX_PATH];
    GetModuleFileName(nullptr, result_char, MAX_PATH);
#elif defined(__linux__)
    char result_char[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result_char, PATH_MAX);
#else
    std::cerr << "Unsupported OS for DistPath\n";
#endif
    std::string result = std::string(result_char);
    std::replace(result.begin(), result.end(), '\\', '/');
    std::string substr         = std::string("dist/bin");
    std::string::size_type pos = std::string(result).find(substr);
    result                     = result.substr(0, pos + 4);
    return result;
  }

}
