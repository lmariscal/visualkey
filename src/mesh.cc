#include "mesh.h"

#include <glad.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "shader.h"

namespace visualkey {

  v3 position = vec3(0.0f);
  quat rotation;

  quat
  GetRotation() {
    return rotation;
  }

  v3
  GetPosition() {
    return position;
  }

  void
  TranslateMesh(v3 pos) {
    position = pos;
  }

  void
  SetRotation(quat value) {
    rotation = value;
  }

  void
  RotateMesh(v3 value) {
    quat identity;
    rotation = rotate(identity, value);
  }

  void
  RotateXMesh(f32 value) {
    rotation = rotate(rotation, { value, 0.0f, 0.0f });
  }

  void
  RotateYMesh(f32 value) {
    rotation = rotate(rotation, { 0.0f, value, 0.0f });
  }

  void
  RotateZMesh(f32 value) {
    rotation = rotate(rotation, { 0.0f, 0.0f, value });
  }

  MeshData *
  CreateMesh(const std::vector<f32> &vertices, const std::vector<u32> &indices, bool is_line) {
    MeshData *data       = new MeshData();
    data->indices_count  = indices.size();
    data->vertices_count = vertices.size();
    data->is_line        = is_line;

    glGenBuffers(1, &data->vbo_array);
    glGenBuffers(1, &data->vbo_element);
    glGenVertexArrays(1, &data->vao);
    glBindVertexArray(data->vao);

    glBindBuffer(GL_ARRAY_BUFFER, data->vbo_array);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->vbo_element);

    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * data->vertices_count, &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(u32) * data->indices_count,
                 &indices[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(f32) * 5, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 5, (void *)(3 * sizeof(f32)));

#ifdef VISUALKEY_DEBUG
    std::cout << "Created mesh " << data->vao << "\n";
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

    return data;
  }

  MeshData *
  CreateMesh(const std::vector<f32> &vertices, bool is_line) {
    std::vector<u32> indices;
    for (int i = 0; i < vertices.size() / 5; ++i)
      indices.push_back(i);
    return CreateMesh(vertices, indices, is_line);
  }

  void
  DestroyMesh(MeshData *data) {
    if (!glfwGetCurrentContext()) return;
    glDeleteBuffers(1, &data->vbo_array);
    glDeleteBuffers(1, &data->vbo_element);
    glDeleteVertexArrays(1, &data->vao);
  }

  void
  DrawMesh(MeshData *data) {
    glBindVertexArray(data->vao);

    ShaderData *shader = GetCurrentShader();
    if (shader->program == GetUberShader()) {
      i32 model_loc = GetLocation(shader, "Model", false);
      m4 model      = mat4(1.0f);
      model         = translate(model, position);
      model         = model * toMat4(rotation);
      SetMat4(shader, model_loc, model);
    }

    if (data->is_line) glDrawElements(GL_LINES, data->indices_count, GL_UNSIGNED_INT, (void *)0);
    else
      glDrawElements(GL_TRIANGLES, data->indices_count, GL_UNSIGNED_INT, (void *)0);

#ifdef VISUALKEY_DEBUG
    glBindVertexArray(0);
#endif
  }

  void
  PolygonMode(bool mode) {
    glPolygonMode(GL_FRONT_AND_BACK, mode ? GL_LINE : GL_FILL);
  }

}
