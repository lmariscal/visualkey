#pragma once

#include "types.h"
#include <mono/jit/jit.h>

namespace visualkey {

  void InitMono();
  void TerminateMono();
  void MonoCompile(const std::string &dir);
  void InitProjectMono(const std::string &dir);
  void MonoStart();
  void MonoUpdate();
  void MonoStop();
  void AddFeatures();

}
