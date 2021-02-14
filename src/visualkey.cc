#include <iostream>
#include <filesystem>
#include <GLFW/glfw3.h>
#include "types.h"
#include "window.h"
#include "mono_script.h"
#include "shader.h"
#include "mesh.h"
#include "image.h"
#include "audio.h"

using namespace visualkey;

void
InitProject(const std::string &dir) {
  if (!std::filesystem::exists(dir)) {
    if (!std::filesystem::exists(std::filesystem::path(dir).parent_path().string())) {
      std::cerr << "Error: Not able to create nested directories for init.\n";
      return;
    }
    std::filesystem::create_directory(dir);
  }
  if (!std::filesystem::is_empty(dir)) {
    bool is_okay = false;
    std::cout << "Directory is not empty.\n";
    while (!is_okay) {
      std::cout << "Continue? No scripts will be modified. [y/n]: ";
      std::string answer;
      std::getline(std::cin, answer);
      if (answer == "y") {
        std::cout << "Continuing...\n";
        is_okay = true;
      } else if (answer == "n") {
        std::cout << "Aborting...\n";
        return;
      } else {
        std::cout << "Please enter either 'y' or 'n'\n";
      }
    }
  }
  InitProjectMono(dir);
}

void
RunProject(const std::string &dir) {
  if (!std::filesystem::exists(dir)) {
    std::cerr << "Error: Directory '" << dir << "' does not exist." << '\n';
    return;
  }

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
uniform vec4 Color;\
uniform sampler2D Texture;\
\
void main() {\
  if (IsTexture)\
    gl_FragColor = texture(Texture, TexCoord);\
  else\
    gl_FragColor = vec4(Color);\
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
    v4 default_color(250.0f / 255.0f, 250.0f / 255.0f, 250.0f / 255.0f, 1.0f);
    m4 model = m4(1.0f);

    DrawShader(uber);
    SetMat4(uber, perspective_loc, perspective);
    SetMat4(uber, model_loc, model);
    SetVec4(uber, color_loc, default_color);

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
}

void
ParseArgs(i32 argc, char const **argv) {
  const std::string help =
    "\
VisualKey v0.1\n\
\n\
Usage:\n\
\tvisualkey [command] <directory>\n\
\n\
Command:\n\
\trun\tRun the directory inside visualkey\n\
\tinit\tInitialize project for VSCode\n\
";

  if (argc == 1) {
    std::cout << help;
    return;
  }

  std::string command = argv[1];
  std::string dir     = argc == 2 ? "." : argv[2];

#if defined(__linux__)
  if (dir[0] != '/' && dir[0] != '.') dir = "./" + dir;
  if (dir[dir.length() - 1] == '/') dir.pop_back();
#endif

  std::cout << "dir: " << dir << '\n';

  if (command == "init") {
    InitProject(dir);
  } else if (command == "run") {
    RunProject(dir);
  } else {
    std::cout << "Unkown command '" << command << "'\n\n";
    std::cout << help;
  }
}

i32
main(i32 argc, char const **argv) {
  ParseArgs(argc, argv);
}
