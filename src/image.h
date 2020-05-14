#pragma once

#include "typedefs.h"

namespace visualkey {

  struct ImageData {
    u32 texture;
  };

  ImageData* CreateImage(std::string path);
  void DestroyImage(ImageData *data);
  void DrawImage(ImageData *data);

}