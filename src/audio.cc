#include "audio.h"

#include <iostream>
#include <irrKlang.h>
#include <filesystem>

#include "utils.h"

namespace visualkey {

  irrklang::ISoundEngine *soundEngine = nullptr;
  std::string dir;

  void
  InitAudio(std::string project_dir) {
    soundEngine = irrklang::createIrrKlangDevice(
      irrklang::ESOD_AUTO_DETECT,
      irrklang::ESEO_MULTI_THREADED | irrklang::ESEO_LOAD_PLUGINS | irrklang::ESEO_USE_3D_BUFFERS);
    dir = project_dir;
    if (dir.at(dir.size() - 1) != '/' && dir.at(dir.size() - 1) != '\\') dir += "/";
  }

  void
  PlayAudio(std::string path, bool loop) {
    if (!std::filesystem::exists(dir + path)) std::cerr << path << " could not be found\n";
    soundEngine->play2D(std::string(dir + path).c_str(), loop);
  }

  void
  TerminateAudio() {
    soundEngine->drop();
  }

}
