#pragma once

#include "types.h"

namespace visualkey {

  void InitAudio(std::string dir);
  void TerminateAudio();
  void PlayAudio(std::string path, bool loop);

}
