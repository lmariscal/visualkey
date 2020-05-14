#include "shader.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>

namespace visualkey {

  ShaderData *current = new ShaderData();
  u32 uber_shader = 0;

  ShaderData*
  GetCurrentShader() {
    return current;
  }

  void
  TerminateShader() {
    delete current;
  }

  void
  SetUberShader(ShaderData *shader) {
    uber_shader = shader->program;
  }

  u32
  GetUberShader() {
    return uber_shader;
  }

  ShaderSource*
  ReadShader(std::string path, ShaderType initial_type) {
    ShaderSource *source = new ShaderSource();
    std::stringstream source_stream[2];
    std::string directory = path.substr(0, path.find_last_of("/") + 1);
    std::string line;
    std::ifstream file(path);

    if (!file.is_open()) {
      std::cerr << "failed to open shader " << path << '\n';
      delete source;
      return nullptr;
    }

    ShaderType type = initial_type;
    i32 line_count = 0;

    while (getline(file, line)) {
      line_count++;

      if (line.find("//") == 0)
        continue;
      if (line == "")
        continue;

      if (line[0] == '#') {
        if (line.find("#include ") != std::string::npos) {
          std::string include_path = line.substr(9, line.length());
          ShaderSource *include_source = ReadShader(directory + include_path, ShaderType::Vertex);
          if (!include_source) {
            std::cerr << "Failed to include " << directory + include_path << "\n\tshader " << path << ":" << line_count << '\n';
            continue;
          }
          if (type != ShaderType::None)
            source_stream[(i32)type] << include_source->vertex << '\n';
          delete include_source;
          continue;
        } else if (line.find("#vertex") != std::string::npos) {
          type = ShaderType::Vertex;
          continue;
        } else if (line.find("#fragment") != std::string::npos) {
          type = ShaderType::Fragment;
          continue;
        } else if (line.find("#ignore") != std::string::npos) {
          type = ShaderType::None;
          continue;
        }
      }

      if (type != ShaderType::None)
        source_stream[(i32)type] << line << '\n';
    }

    file.close();
#ifdef VISUALKEY_DEBUG
    std::cout << "Loaded shader \"" << path << "\"\n";
#endif

    source->vertex = source_stream[0].str();
    source->fragment = source_stream[1].str();
    return source;
  }

  ShaderSource*
  ReadShader(const std::string &vertex, const std::string &fragment) {
    ShaderSource *source = new ShaderSource();
    source->vertex = vertex;
    source->fragment = fragment;
    return source;
  }

  void
  StatusProgram(u32 program_id) {
    i32 status;
    glGetProgramiv(program_id, GL_LINK_STATUS, &status);
    if (status == GL_TRUE)
      return;

    i32 log_length;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<char> message_array(log_length);
    glGetProgramInfoLog(program_id, log_length, nullptr, &message_array[0]);
    std::string message(begin(message_array), end(message_array));

    std::cerr << "Failed to link shader program:\n" << message << '\n';
  }

  void
  StatusShader(u32 shader_id, ShaderType type) {
    i32 status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
      return;

    i32 log_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<char> message_array(log_length);
    glGetShaderInfoLog(shader_id, log_length, nullptr, &message_array[0]);
    std::string message(begin(message_array), end(message_array));

    std::cerr << "Failed to compile " << (type == ShaderType::Vertex ? "Vertex" : "Fragment") << " shader:\n" << message << '\n';
  }

  ShaderData*
  CreateShader(ShaderSource *source) {
    ShaderData *data = new ShaderData();

    u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_source = source->vertex.c_str();
    glShaderSource(vertex, 1, &vertex_source, nullptr);
    glCompileShader(vertex);
    StatusShader(vertex, ShaderType::Vertex);

    u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_source = source->fragment.c_str();
    glShaderSource(fragment, 1, &fragment_source, nullptr);
    glCompileShader(fragment);
    StatusShader(fragment, ShaderType::Fragment);

    data->program = glCreateProgram();
    glAttachShader(data->program, vertex);
    glAttachShader(data->program, fragment);
    glLinkProgram(data->program);
    StatusProgram(data->program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
#ifdef VISUALKEY_DEBUG
    std::cout << "Created shader " << data->program << "\n";
#endif

    return data;
  }

  ShaderData*
  CreateShader(std::string path) {
    ShaderSource *source = ReadShader(path);
    ShaderData *data = CreateShader(source);
    delete source;
    return data;
  }

  void
  DestroyShader(ShaderData *data) {
#ifdef VISUALKEY_DEBUG
    std::cout << "Deleted shader " << data->program << "\n";
#endif
    if (!glfwGetCurrentContext())
      return;
    glDeleteProgram(data->program);
  }

  void
  DrawShader(ShaderData *data) {
    glUseProgram(data->program);
    current->program = data->program;
  }

  i32
  GetLocation(ShaderData *data, const std::string &name, bool msg) {
#ifdef VISUALKEY_DEBUG
    DrawShader(data);
#endif
    i32 pos = glGetUniformLocation(data->program, name.c_str());
#ifdef VISUALKEY_DEBUG
    if (pos == -1 && msg)
      std::cerr << "Failed to locate \"" << name << "\" uniform\n";
#endif
    return pos;
  }

  void
  SetVec2(ShaderData *data, i32 location, v2 &vec) {
    DrawShader(data);
    glUniform2fv(location, 1, &vec[0]);
  }

  void
  SetVec3(ShaderData *data, i32 location, v3 &vec) {
    DrawShader(data);
    glUniform3fv(location, 1, &vec[0]);
  }

  void
  SetVec4(ShaderData *data, i32 location, v4 &vec) {
    DrawShader(data);
    glUniform4fv(location, 1, &vec[0]);
  }

  void
  SetMat3(ShaderData *data, i32 location, m3 &mat) {
    DrawShader(data);
    glUniformMatrix3fv(location, 1, false, &mat[0][0]);
  }

  void
  SetMat4(ShaderData *data, i32 location, m4 &mat) {
    DrawShader(data);
    glUniformMatrix4fv(location, 1, false, &mat[0][0]);
  }

  void
  SetInt(ShaderData *data, i32 location, i32 val) {
    DrawShader(data);
    glUniform1i(location, val);
  }

}
