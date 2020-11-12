#pragma once

#include "types.h"
#include <vector>

namespace visualkey {

  struct MeshData {
    u32 vao;
    u32 vbo_array;
    u32 vbo_element;
    u32 vertices_count;
    u32 indices_count;
    bool is_line;
  };

  MeshData *CreateMesh(const std::vector<f32> &vertices,
                       const std::vector<u32> &indices,
                       bool is_line = false);
  MeshData *CreateMesh(const std::vector<f32> &vertices, bool is_line = false);
  quat GetRotation();
  v3 GetPosition();
  void SetRotation(quat value);
  void RotateMesh(v3 value);
  void RotateXMesh(f32 value);
  void RotateYMesh(f32 value);
  void RotateZMesh(f32 value);
  void TranslateMesh(vec3 position);
  void DestroyMesh(MeshData *data);
  void DrawMesh(MeshData *data);
  void PolygonMode(bool mode);

}
