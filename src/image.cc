#include "image.h"
#include "shader.h"

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
    uchar *data = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (!data) {
      std::cerr << "Failed to load image " << path << '\n';
      return image_data;
    }

    glGenTextures(1, &image_data->texture);
    glBindTexture(GL_TEXTURE_2D, image_data->texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
    ShaderData *shader = GetCurrentShader();
    i32 color_loc      = GetLocation(shader, "Color");

    if (!data) {
      glBindTexture(GL_TEXTURE_2D, 0);
      return;
    }
    v4 identity = { 0.0f, 0.0f, 0.0f, 0.0f };
    SetVec4(shader, color_loc, identity);
    glBindTexture(GL_TEXTURE_2D, data->texture);
  }

}
