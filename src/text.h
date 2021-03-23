#pragma once

#include "types.h"

namespace visualkey {

  struct Character {
    u32 TextureID;
    ivec2 Size;
    ivec2 Bearing;
    u32 Advance;
  };

  void InitText();
  void TerminateText();
  void RenderText(std::string text, float scale, v3 color);

}
