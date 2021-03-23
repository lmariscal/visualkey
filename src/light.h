#pragma once

#include "types.h"
#include <vector>

namespace visualkey {

  void EnableLightSystem();
  void DisableLightSystem();
  void DestroyLightSource(u32 id);
  u32 CreateLightSource(v3 position);
  void ChangeLightPosition(u32 id, v3 new_pos);

}
