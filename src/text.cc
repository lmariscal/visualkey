#include "text.h"

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad.h>
#include <map>
#include <GLFW/glfw3.h>

#include "utils.h"
#include "shader.h"
#include "mesh.h"
#include "light.h"
#include "window.h"

namespace visualkey {

  std::map<GLchar, Character> characters;
  unsigned int VAO, VBO;
  ShaderData *text_shader = nullptr;

  void
  InitText() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) { std::cout << "Could not init FreeType Library\n"; }

    std::string font_name = DistPath() + "/etc/fonts/JetBrainsMono-Regular.ttf";
    if (font_name.empty()) { std::cout << "Failed to load JetBrains Mono\n"; }

    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
      std::cout << "Failed to load Font\n";
    } else {
      FT_Set_Pixel_Sizes(face, 0, 48);

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      for (uchar c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
          std::cout << "Failed to load Glyph\n";
          continue;
        }
        u32 texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = { texture,
                                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                static_cast<unsigned int>(face->glyph->advance.x) };
        characters.insert(std::pair<char, Character>(c, character));
      }
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    ShaderSource *text_source = ReadShader(
      "\
#version 330 core\n\
\
layout (location = 0) in vec3 vPos;\
layout (location = 1) in vec2 vTexCoord;\
layout (location = 2) in vec3 vNormal;\
\
out vec2 TexCoord;\
\
uniform mat4 Projection;\
uniform mat4 Model;\
\
void main() {\
  gl_Position = Projection * Model * vec4(vPos, 1.0);\
  TexCoord = vTexCoord;\
}\
      ",
      "\
#version 330 core\n\
\
in vec2 TexCoord;\
\
uniform vec4 Color;\
uniform sampler2D Texture;\
\
layout (location = 0) out vec4 OutColor;\
\
void main() {\
\
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(Texture, TexCoord).r);\
  OutColor = Color * sampled;\
}\
      ");
    text_shader = CreateShader(text_source);
    delete text_source;
  }

  void
  TerminateText() {
    for (auto const &[key, val] : characters) {
      if (!glfwGetCurrentContext()) return;
      glDeleteTextures(1, &val.TextureID);
    }
    DestroyShader(text_shader);
  }

  void
  RenderText(std::string text, float scale, v3 color) {
    scale                      = scale / 10.0f;
    ShaderData *current_shader = GetCurrentShader();

    DrawShader(text_shader);
    i32 color_loc = GetLocation(text_shader, "Color", false);
    v4 color_a    = v4(color, 1.0f);
    SetVec4(text_shader, color_loc, color_a);

    i32 model_loc = GetLocation(text_shader, "Model", false);
    v3 position   = GetPosition();
    quat rotation = GetRotation();
    m4 model      = mat4(1.0f);
    model         = translate(model, position);
    model         = model * toMat4(rotation);
    SetMat4(text_shader, model_loc, model);

    v2i size(0);
    glfwGetWindowSize(glfwGetCurrentContext(), &size.x, &size.y);
    m4 perspective      = ortho(-(size.x / 2.0f), size.x / 2.0f, -(size.y / 2.0f), size.y / 2.0f);
    i32 perspective_loc = GetLocation(text_shader, "Projection");
    SetMat4(text_shader, perspective_loc, perspective);

    f32 x = 0;
    // Inefficient to create a mesh for **each** character and render **each** character with a draw
    // call
    std::string::const_iterator c;
    glActiveTexture(GL_TEXTURE0);
    for (c = text.begin(); c != text.end(); c++) {
      Character ch = characters[*c];

      f32 xpos = x + ch.Bearing.x * scale;
      f32 ypos = (ch.Size.y - ch.Bearing.y) * scale;
      f32 w    = ch.Size.x * scale;
      f32 h    = ch.Size.y * scale;

      std::vector<f32> vertices = { xpos,     ypos + h, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                    xpos,     ypos,     0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                    xpos + w, ypos,     0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                    xpos,     ypos + h, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                    xpos + w, ypos,     0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                    xpos + w, ypos + h, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };

      MeshData *mesh = CreateMesh(vertices);

      glBindTexture(GL_TEXTURE_2D, ch.TextureID);
      DrawMesh(mesh);

      DestroyMesh(mesh);
      x += (ch.Advance >> 6) * scale;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    DrawShader(current_shader);
  }

}
