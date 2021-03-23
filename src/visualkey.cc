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
#include "light.h"
#include "text.h"

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
  InitText();
  InitAudio(dir);
  InitMono();

  ShaderSource *uber_source = ReadShader(
    "\
#version 330 core\n\
\
layout (location = 0) in vec3 vPos;\
layout (location = 1) in vec2 vTexCoord;\
layout (location = 2) in vec3 vNormal;\
\
out vec2 TexCoord;\
out vec3 Normal;\
out vec3 FragPos;\
\
uniform mat4 Projection;\
uniform mat4 View;\
uniform mat4 Model;\
\
void main() {\
  gl_Position = Projection * View * Model * vec4(vPos, 1.0);\
\
  FragPos = vec3(Model * vec4(vPos, 1.0));\
  TexCoord = vTexCoord;\
  Normal = mat3(transpose(inverse(Model))) * vNormal;\
}\
  ",
    "\
#version 330 core\n\
\
in vec2 TexCoord;\
in vec3 Normal;\
in vec3 FragPos;\
\
uniform bool IsTexture;\
uniform vec4 Color;\
uniform int NumberLightSources;\
uniform vec3 LightSources[256];\
uniform sampler2D Texture;\
uniform float AmbientLight;\
\
layout (location = 0) out vec4 OutColor;\
\
void main() {\
  vec4 result = texture(Texture, TexCoord) + Color;\
  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);\
\
  vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);\
\
  vec3 norm = normalize(Normal);\
  for (int i = 0; i < NumberLightSources; ++i) {\
    vec3 lightPos = LightSources[i];\
    vec3 lightDir = normalize(lightPos - FragPos);\
    float diff = max(dot(norm, lightDir), 0.0);\
    diffuse += diff * lightColor;\
  }\
\
  result.xyz *= AmbientLight + diffuse;\
\
  OutColor = result;\
}\
  ");
  ShaderData *uber = CreateShader(uber_source);
  delete uber_source;
  SetUberShader(uber);

  // i32 perspective_loc = GetLocation(uber, "Perspective");
  i32 color_loc         = GetLocation(uber, "Color");
  i32 model_loc         = GetLocation(uber, "Model");
  i32 view_loc          = GetLocation(uber, "View");
  i32 ambient_light_loc = GetLocation(uber, "AmbientLight");

  MonoCompile(dir);
  MonoStart();

  while (WindowIsOpen(GetDefaultWindow())) {
    NewFrame();

    TranslateMesh({ 0.0f, 0.0f, 0.0f });
    quat identity(1.0, 0.0, 0.0, 0.0);
    SetRotation(identity);
    DrawImage(nullptr);

    WindowData *data = GetWindowsFirst();
    if (!data) break;
    MakeCurrent(data);

    // v2i size(0);
    // glfwGetWindowSize(glfwGetCurrentContext(), &size.x, &size.y);
    // m4 perspective = IsOrtho()
    //   ? ortho(-(size.x / 2.0f), size.x / 2.0f, -(size.y / 2.0f), size.y / 2.0f)
    //   : glm::perspective(radians(106.0f), (f32)size.x / (f32)size.y, 0.1f, 10000.0f);
    v4 default_color(250.0f / 255.0f, 250.0f / 255.0f, 250.0f / 255.0f, 1.0f);
    m4 m4_indentity = m4(1.0f);

    DrawShader(uber);
    // SetMat4(uber, perspective_loc, perspective);
    SetFloat(uber, ambient_light_loc, 1.0f);
    SetMat4(uber, view_loc, m4_indentity);
    SetVec4(uber, color_loc, default_color);
    DisableLightSystem();

    MonoUpdate();

    SwapAllBuffers();
  }

  DestroyShader(uber);
  TerminateShader();
  MonoStop();
  TerminateAudio();
  TerminateMono();
  TerminateText();
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
