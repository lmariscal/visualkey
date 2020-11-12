#pragma once

#include "types.h"

#include <vector>

namespace visualkey {

  enum class ShaderType : i32 { None = -1, Vertex = 0, Fragment = 1 };

  struct ShaderSource {
    std::string vertex;
    std::string fragment;
  };

  struct ShaderData {
    u32 program;
  };

  ShaderSource *ReadShader(std::string path, ShaderType initial_type = ShaderType::None);
  ShaderSource *ReadShader(const std::string &vertex, const std::string &fragment);
  ShaderData *GetCurrentShader();
  ShaderData *CreateShader(ShaderSource *source);
  ShaderData *CreateShader(std::string path);
  i32 GetLocation(ShaderData *data, const std::string &name, bool msg = true);
  u32 GetUberShader();
  void SetUberShader(ShaderData *data);
  void DestroyShader(ShaderData *data);
  void DrawShader(ShaderData *data);
  void TerminateShader();
  void SetInt(ShaderData *data, i32 location, i32 val);
  void SetVec2(ShaderData *data, i32 location, v2 &vec);
  void SetVec3(ShaderData *data, i32 location, v3 &vec);
  void SetVec4(ShaderData *data, i32 location, v4 &vec);
  void SetMat3(ShaderData *data, i32 location, m3 &mat);
  void SetMat4(ShaderData *data, i32 location, m4 &mat);

}
