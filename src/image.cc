#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad.h>
#include <iostream>
#include <GLFW/glfw3.h>

namespace visualkey {

  ImageData *
  CreateImage(std::string path) {
    ImageData *image_data = new ImageData();
    i32 width, height, channels;
    uchar *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
      std::cerr << "Failed to load image " << path << '\n';
      return image_data;
    }

    glGenTextures(1, &image_data->texture);
    glBindTexture(GL_TEXTURE_2D, image_data->texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

#ifdef VISUALKEY_DEBUG
    glBindTexture(GL_TEXTURE_2D, 0);
#endif

    stbi_image_free(data);
    return image_data;
  }

  void
  DestroyImage(ImageData *data) {
    if (!glfwGetCurrentContext()) return;
    glDeleteTextures(1, &data->texture);
  }

  void
  DrawImage(ImageData *data) {
    glBindTexture(GL_TEXTURE_2D, data->texture);
  }

}
