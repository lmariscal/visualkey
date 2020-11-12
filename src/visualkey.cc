#include <iostream>
#include <GLFW/glfw3.h>
#include "types.h"
#include "window.h"
#include "mono_script.h"
#include "shader.h"
#include "mesh.h"
#include "image.h"
#include "audio.h"

using namespace visualkey;

i32
main(i32 argc, char const **argv) {
  if (argc == 1) {
    std::cout << "\
Usage:\n\
  visualkey [DIRECTORY]\n\
";
    return 0;
  }

  std::string dir = argv[1];

  InitGFX();
  InitAudio(dir);
  InitMono();
  MonoCompile(dir);
  MonoStart();

  ShaderSource *uber_source = ReadShader(
    "\
#version 330 core\n\
\
layout (location = 0) in vec3 vPos;\
layout (location = 1) in vec2 vTexCoord;\
\
out vec2 TexCoord;\
\
uniform mat4 Perspective;\
uniform mat4 Model;\
\
void main() {\
  gl_Position = Perspective * Model * vec4(vPos, 1.0);\
  TexCoord = vTexCoord;\
}\
  ",
    "\
#version 330 core\n\
\
in vec2 TexCoord;\
\
uniform bool IsTexture;\
uniform vec3 Color;\
uniform sampler2D Texture;\
\
void main() {\
  if (IsTexture)\
    gl_FragColor = texture(Texture, TexCoord);\
  else\
    gl_FragColor = vec4(Color, 1.0f);\
}\
  ");
  ShaderData *uber = CreateShader(uber_source);
  delete uber_source;
  SetUberShader(uber);

  i32 perspective_loc = GetLocation(uber, "Perspective");
  i32 color_loc       = GetLocation(uber, "Color");
  i32 model_loc       = GetLocation(uber, "Model");

  while (WindowIsOpen(GetFocusedWindow())) {
    NewFrame();
    TranslateMesh({ 0.0f, 0.0f, 0.0f });
    RotateMesh({ 0.0f, 0.0f, 0.0f });

    v2i size(0);
    glfwGetWindowSize(glfwGetCurrentContext(), &size.x, &size.y);
    m4 perspective = IsOrtho()
      ? ortho(-(size.x / 2.0f), size.x / 2.0f, -(size.y / 2.0f), size.y / 2.0f)
      : glm::perspective(radians(106.0f), (f32)size.x / (f32)size.y, 0.1f, 10000.0f);
    v3 default_color(250.0f / 255.0f, 250.0f / 255.0f, 250.0f / 255.0f);
    m4 model = m4(1.0f);

    DrawShader(uber);
    SetMat4(uber, perspective_loc, perspective);
    SetMat4(uber, model_loc, model);
    SetVec3(uber, color_loc, default_color);

    MonoUpdate();
    SwapAllBuffers();
  }

  DestroyShader(uber);
  TerminateShader();
  MonoStop();
  TerminateAudio();
  TerminateMono();
  TerminateGFX();
  delete uber;
  return 0;
}
