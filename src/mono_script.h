#pragma once

#include "typedefs.h"
#include <mono/jit/jit.h>

namespace visualkey {

  void InitMono();
  void TerminateMono();
  void MonoCompile(std::string dir);
  void MonoStart();
  void MonoUpdate();
  void MonoStop();
  void AddFeatures();

}