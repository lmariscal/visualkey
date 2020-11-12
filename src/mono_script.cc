#include "mono_script.h"

#include <iostream>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "window.h"
#include "iomanager.h"
#include "mesh.h"
#include "shader.h"
#include "image.h"
#include "utils.h"
#include "audio.h"

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace visualkey {

  std::string path;
  MonoDomain *domain           = nullptr;
  MonoAssembly *assembly       = nullptr;
  MonoAssembly *lib_assembly   = nullptr;
  MonoImage *image             = nullptr;
  MonoImage *lib_image         = nullptr;
  MonoClass *application_class = nullptr;
  MonoObject *application      = nullptr;

  void
  InitMono() {
    domain = mono_jit_init("VisualKey");
    if (!domain) std::cerr << "Failed to init mono_jit\n";
    AddFeatures();
  }

  void
  TerminateMono() {
    mono_jit_cleanup(domain);
  }

  void
  MonoCompile(std::string dir) {
    path = dir;
    if (dir.at(dir.size() - 1) != '/' && dir.at(dir.size() - 1) != '\\') path += "/";

#if defined(WIN32) || defined(_WIN32)
#if defined(VISUALKEY_DEBUG)
    std::string command = ExePath() + "/../mono/mcs " + path + "*.cs /out:" + path +
      "App.dll /target:library /debug /optimize /w:4 /nologo /reference:" + ExePath() +
      "/../api/Debug/VisualKey.dll";
#else
    std::string command = ExePath() + "/../mono/mcs " + path + "*.cs /out:" + path +
      "App.dll /target:library /debug /optimize /w:4 /nologo /reference:" + ExePath() +
      "/../api/Release/VisualKey.dll";
#endif
    i32 ret = system(command.c_str());
    if (ret != 0) {
      std::cerr << "Failed to compile\nExiting program...\n";
      TerminateMono();
      exit(1);
    }
#elif defined(__linux__)
#if defined(VISUALKEY_DEBUG)
    std::string command = "mcs " + path + "*.cs /out:" + path +
      "App.dll /target:library /debug /optimize /w:4 /nologo /reference:" + ExePath() +
      "/../api/Debug/VisualKey.dll";
#else
    std::string command = "mcs " + path + "*.cs /out:" + path +
      "App.dll /target:library /debug /optimize /w:4 /nologo /reference:" + ExePath() +
      "/../api/Release/VisualKey.dll";
#endif
    i32 ret = system(command.c_str());
    if (ret != 0) {
      std::cerr << "Failed to compile\nExiting program...\n";
      TerminateMono();
      exit(1);
    }
#else
    std::cerr << "Unsupported OS for MonoCompile\n";
#endif

    std::string name = path;
    name += "App.dll";

    mono_set_dirs(std::string(ExePath() + "/../lib").c_str(),
                  std::string(ExePath() + "/../etc").c_str());

    mono_set_assemblies_path(ExePath().c_str());

#if defined(__linux__)
    mono_dllmap_insert(NULL, "i:System.Native", NULL, "libmono-native.so", NULL);
#endif

    assembly = mono_domain_assembly_open(domain, name.c_str());
    if (!assembly) {
      std::cerr << "Failed to get assembly from mono_domain_assembly_open main app\n";
      return;
    }

    image = mono_assembly_get_image(assembly);
    if (!image) {
      std::cerr << "Failed to get image from mono_assembly_get_image main assembly\n";
      return;
    }

    std::string lib = ExePath() + "/../api/Release/VisualKey.dll";

    lib_assembly = mono_domain_assembly_open(domain, lib.c_str());
    if (!lib_assembly) {
      std::cerr << "Failed to get assembly from mono_domain_assembly_open lib\n";
      return;
    }

    lib_image = mono_assembly_get_image(lib_assembly);
    if (!lib_image) {
      std::cerr << "Failed to get image from mono_assembly_get_image lib assembly\n";
      return;
    }

    application_class = mono_class_from_name(image, "", "App");
    if (!application_class) {
      std::cerr << "Failed to get class from mono_class_from_name\n";
      return;
    }

    application = mono_object_new(domain, application_class);
    if (!application) {
      std::cerr << "Failed to get application for mono_object_new\n";
      return;
    }

    mono_runtime_object_init(application);
  }

  void
  MonoStart() {
    MonoMethodDesc *type_method_desc = nullptr;
    std::string type_method_desc_str = "App:Start()";
    type_method_desc                 = mono_method_desc_new(type_method_desc_str.c_str(), false);
    if (!type_method_desc) {
      std::cerr << "Failed to get type_method_desc from mono_method_desc_new\n";
      return;
    }

    MonoMethod *method = nullptr;
    method             = mono_method_desc_search_in_image(type_method_desc, image);
    if (!method) return;

    mono_runtime_invoke(method, application, nullptr, nullptr);
  }

  void
  MonoUpdate() {
    MonoMethodDesc *type_method_desc = nullptr;
    std::string type_method_desc_str = "App:Update()";
    type_method_desc                 = mono_method_desc_new(type_method_desc_str.c_str(), false);
    if (!type_method_desc) {
      std::cerr << "Failed to get type_method_desc from mono_method_desc_new\n";
      return;
    }

    MonoMethod *method = nullptr;
    method             = mono_method_desc_search_in_image(type_method_desc, image);
    if (!method) return;

    mono_runtime_invoke(method, application, nullptr, nullptr);
  }

  void
  MonoStop() {
    MonoMethodDesc *type_method_desc = nullptr;
    std::string type_method_desc_str = "App:Stop()";
    type_method_desc                 = mono_method_desc_new(type_method_desc_str.c_str(), false);
    if (!type_method_desc) {
      std::cerr << "Failed to get type_method_desc from mono_method_desc_new\n";
      return;
    }

    MonoMethod *method = nullptr;
    method             = mono_method_desc_search_in_image(type_method_desc, image);
    if (!method) return;

    mono_runtime_invoke(method, application, nullptr, nullptr);
  }

  v2
  ToVec2(MonoObject *obj) {
    v2 result(0);
    MonoClass *klass        = mono_object_get_class(obj);
    MonoClassField *field_x = mono_class_get_field_from_name(klass, "x");
    MonoClassField *field_y = mono_class_get_field_from_name(klass, "y");

    if (!field_x || !field_y) {
      std::cerr << "obj is not a v2\n";
      return result;
    }

    mono_field_get_value(obj, field_x, &result.x);
    mono_field_get_value(obj, field_y, &result.y);
    return result;
  }

  v3
  ToVec3(MonoObject *obj) {
    v3 result(0);
    MonoClass *klass        = mono_object_get_class(obj);
    MonoClassField *field_x = mono_class_get_field_from_name(klass, "x");
    MonoClassField *field_y = mono_class_get_field_from_name(klass, "y");
    MonoClassField *field_z = mono_class_get_field_from_name(klass, "z");

    if (!field_x || !field_y || !field_z) {
      std::cerr << "obj is not a v3\n";
      return result;
    }

    mono_field_get_value(obj, field_x, &result.x);
    mono_field_get_value(obj, field_y, &result.y);
    mono_field_get_value(obj, field_z, &result.z);
    return result;
  }

  quat
  ToQuat(MonoObject *obj) {
    quat result;
    MonoClass *klass        = mono_object_get_class(obj);
    MonoClassField *field_x = mono_class_get_field_from_name(klass, "x");
    MonoClassField *field_y = mono_class_get_field_from_name(klass, "y");
    MonoClassField *field_z = mono_class_get_field_from_name(klass, "z");
    MonoClassField *field_w = mono_class_get_field_from_name(klass, "w");

    if (!field_x || !field_y || !field_z || !field_w) {
      std::cerr << "obj is not a quat\n";
      return result;
    }

    mono_field_get_value(obj, field_x, &result.x);
    mono_field_get_value(obj, field_y, &result.y);
    mono_field_get_value(obj, field_z, &result.z);
    mono_field_get_value(obj, field_w, &result.w);
    return result;
  }

  v4
  ToVec4(MonoObject *obj) {
    v4 result(0);
    MonoClass *klass        = mono_object_get_class(obj);
    MonoClassField *field_x = mono_class_get_field_from_name(klass, "x");
    MonoClassField *field_y = mono_class_get_field_from_name(klass, "y");
    MonoClassField *field_z = mono_class_get_field_from_name(klass, "z");
    MonoClassField *field_w = mono_class_get_field_from_name(klass, "w");

    if (!field_x || !field_y || !field_z || !field_w) {
      std::cerr << "obj is not a v4\n";
      return result;
    }

    mono_field_get_value(obj, field_x, &result.x);
    mono_field_get_value(obj, field_y, &result.y);
    mono_field_get_value(obj, field_z, &result.z);
    mono_field_get_value(obj, field_w, &result.w);
    return result;
  }

  m3
  ToMat3(MonoObject *obj) {
    m3 result(0);
    MonoClass *klass     = mono_object_get_class(obj);
    MonoMethod *to_array = mono_class_get_method_from_name(klass, "ToArray", 0);
    MonoArray *array     = (MonoArray *)mono_runtime_invoke(to_array, obj, nullptr, nullptr);

    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
        result[i][j] = mono_array_get(array, f32, (i * 3) + j);

    return result;
  }

  m4
  ToMat4(MonoObject *obj) {
    m4 result(0);
    MonoClass *klass     = mono_object_get_class(obj);
    MonoMethod *to_array = mono_class_get_method_from_name(klass, "ToArray", 0);
    MonoArray *array     = (MonoArray *)mono_runtime_invoke(to_array, obj, nullptr, nullptr);

    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        result[i][j] = mono_array_get(array, f32, (i * 4) + j);

    return result;
  }

  MonoObject *
  FromVec2(v2 &vec) {
    MonoClass *vec2_klass = mono_class_from_name(lib_image, "VisualKey", "Vec2");
    MonoObject *vec2_obj  = mono_object_new(domain, vec2_klass);

    MonoClassField *field_x = mono_class_get_field_from_name(vec2_klass, "x");
    MonoClassField *field_y = mono_class_get_field_from_name(vec2_klass, "y");
    mono_field_set_value(vec2_obj, field_x, &vec.x);
    mono_field_set_value(vec2_obj, field_y, &vec.y);

    return vec2_obj;
  }

  MonoObject *
  FromVec3(v3 &vec) {
    MonoClass *vec3_klass = mono_class_from_name(lib_image, "VisualKey", "Vec3");
    MonoObject *vec3_obj  = mono_object_new(domain, vec3_klass);

    MonoClassField *field_x = mono_class_get_field_from_name(vec3_klass, "x");
    MonoClassField *field_y = mono_class_get_field_from_name(vec3_klass, "y");
    MonoClassField *field_z = mono_class_get_field_from_name(vec3_klass, "z");
    mono_field_set_value(vec3_obj, field_x, &vec.x);
    mono_field_set_value(vec3_obj, field_y, &vec.y);
    mono_field_set_value(vec3_obj, field_z, &vec.z);

    return vec3_obj;
  }

  MonoObject *
  FromVec4(v4 &vec) {
    MonoClass *vec4_klass = mono_class_from_name(lib_image, "VisualKey", "Vec4");
    MonoObject *vec4_obj  = mono_object_new(domain, vec4_klass);

    MonoClassField *field_x = mono_class_get_field_from_name(vec4_klass, "x");
    MonoClassField *field_y = mono_class_get_field_from_name(vec4_klass, "y");
    MonoClassField *field_z = mono_class_get_field_from_name(vec4_klass, "z");
    MonoClassField *field_w = mono_class_get_field_from_name(vec4_klass, "w");
    mono_field_set_value(vec4_obj, field_x, &vec.x);
    mono_field_set_value(vec4_obj, field_y, &vec.y);
    mono_field_set_value(vec4_obj, field_z, &vec.z);
    mono_field_set_value(vec4_obj, field_w, &vec.z);

    return vec4_obj;
  }

  MonoObject *
  FromQuat(quat &q) {
    MonoClass *quat_klass = mono_class_from_name(lib_image, "VisualKey", "Quat");
    MonoObject *quat_obj  = mono_object_new(domain, quat_klass);

    MonoClassField *field_x = mono_class_get_field_from_name(quat_klass, "x");
    MonoClassField *field_y = mono_class_get_field_from_name(quat_klass, "y");
    MonoClassField *field_z = mono_class_get_field_from_name(quat_klass, "z");
    MonoClassField *field_w = mono_class_get_field_from_name(quat_klass, "w");
    mono_field_set_value(quat_obj, field_x, &q.x);
    mono_field_set_value(quat_obj, field_y, &q.y);
    mono_field_set_value(quat_obj, field_z, &q.z);
    mono_field_set_value(quat_obj, field_w, &q.z);

    return quat_obj;
  }

  MonoObject *
  FromWindowData(WindowData *data) {
    MonoClass *window_klass = mono_class_from_name(lib_image, "VisualKey", "Window");
    MonoObject *window_obj  = mono_object_new(domain, window_klass);

    MonoClassField *field_window = mono_class_get_field_from_name(window_klass, "window");
    mono_field_set_value(window_obj, field_window, &data->window);

    return window_obj;
  }

  WindowData *
  ToWindowData(MonoObject *obj) {
    WindowData *data             = new WindowData();
    MonoClass *klass             = mono_object_get_class(obj);
    MonoClassField *field_window = mono_class_get_field_from_name(klass, "window");

    if (!field_window) {
      std::cerr << "obj is not a Window\n";
      return data;
    }

    mono_field_get_value(obj, field_window, &data->window);

    return data;
  }

  void
  CreateWindowMono(u32 width, u32 height, MonoString *title, MonoObject *recipient) {
    std::string u8_title    = mono_string_to_utf8(title);
    WindowData *window_data = WindowCreate(width, height, u8_title);

    MonoClass *klass             = mono_object_get_class(recipient);
    MonoClassField *field_window = mono_class_get_field_from_name(klass, "window");
    mono_field_set_value(recipient, field_window, &window_data->window);

    delete window_data;
  }

  void
  MakeCurrentWindowMono(MonoObject *window) {
    WindowData *data = ToWindowData(window);
    MakeCurrent(data);
    delete data;
  }

  void
  DestroyWindowMono(MonoObject *obj) {
    WindowData *window_data = ToWindowData(obj);
    DestroyWindow(window_data);
    delete window_data;
  }

  MonoObject *
  FromMeshData(MeshData *data) {
    MonoClass *mesh_klass = mono_class_from_name(lib_image, "VisualKey", "Mesh");
    MonoObject *mesh_obj  = mono_object_new(domain, mesh_klass);

    MonoClassField *field_vao         = mono_class_get_field_from_name(mesh_klass, "vao");
    MonoClassField *field_vbo_array   = mono_class_get_field_from_name(mesh_klass, "vboArray");
    MonoClassField *field_vbo_element = mono_class_get_field_from_name(mesh_klass, "vboElement");
    MonoClassField *field_vertices_count =
      mono_class_get_field_from_name(mesh_klass, "verticesCount");
    MonoClassField *field_indices_count =
      mono_class_get_field_from_name(mesh_klass, "indicesCount");
    MonoClassField *field_is_line = mono_class_get_field_from_name(mesh_klass, "isLine");
    mono_field_set_value(mesh_obj, field_vao, &data->vao);
    mono_field_set_value(mesh_obj, field_vbo_array, &data->vbo_array);
    mono_field_set_value(mesh_obj, field_vbo_element, &data->vbo_element);
    mono_field_set_value(mesh_obj, field_vertices_count, &data->vertices_count);
    mono_field_set_value(mesh_obj, field_indices_count, &data->indices_count);
    mono_field_set_value(mesh_obj, field_is_line, &data->is_line);

    return mesh_obj;
  }

  MeshData *
  ToMeshData(MonoObject *obj) {
    MeshData *data   = new MeshData();
    MonoClass *klass = mono_object_get_class(obj);

    MonoClassField *field_vao            = mono_class_get_field_from_name(klass, "vao");
    MonoClassField *field_vbo_array      = mono_class_get_field_from_name(klass, "vboArray");
    MonoClassField *field_vbo_element    = mono_class_get_field_from_name(klass, "vboElement");
    MonoClassField *field_vertices_count = mono_class_get_field_from_name(klass, "verticesCount");
    MonoClassField *field_indices_count  = mono_class_get_field_from_name(klass, "indicesCount");
    MonoClassField *field_is_line        = mono_class_get_field_from_name(klass, "isLine");

    if (!field_vao || !field_vbo_array || !field_vbo_element || !field_vertices_count ||
        !field_indices_count || !field_is_line) {
      std::cerr << "obj is not a Mesh\n";
      return data;
    }

    mono_field_get_value(obj, field_vao, &data->vao);
    mono_field_get_value(obj, field_vbo_array, &data->vbo_array);
    mono_field_get_value(obj, field_vbo_element, &data->vbo_element);
    mono_field_get_value(obj, field_vertices_count, &data->vertices_count);
    mono_field_get_value(obj, field_indices_count, &data->indices_count);
    mono_field_get_value(obj, field_is_line, &data->is_line);

    return data;
  }

  void
  CreateMeshIndicesMono(MonoArray *vertices_array,
                        MonoArray *indices_array,
                        bool is_line,
                        MonoObject *recipient) {
    i32 vertices_length = mono_array_length(vertices_array);
    std::vector<f32> vertices;
    for (i32 i = 0; i < vertices_length; ++i)
      vertices.push_back(mono_array_get(vertices_array, f32, i));

    i32 indices_length = mono_array_length(indices_array);
    std::vector<u32> indices;
    for (i32 i = 0; i < indices_length; ++i)
      indices.push_back(mono_array_get(indices_array, u32, i));

    MeshData *mesh_data = CreateMesh(vertices, indices, is_line);

    MonoClass *klass                     = mono_object_get_class(recipient);
    MonoClassField *field_vao            = mono_class_get_field_from_name(klass, "vao");
    MonoClassField *field_vbo_array      = mono_class_get_field_from_name(klass, "vboArray");
    MonoClassField *field_vbo_element    = mono_class_get_field_from_name(klass, "vboElement");
    MonoClassField *field_vertices_count = mono_class_get_field_from_name(klass, "verticesCount");
    MonoClassField *field_indices_count  = mono_class_get_field_from_name(klass, "indicesCount");
    MonoClassField *field_is_line        = mono_class_get_field_from_name(klass, "isLine");

    mono_field_set_value(recipient, field_vao, &mesh_data->vao);
    mono_field_set_value(recipient, field_vbo_array, &mesh_data->vbo_array);
    mono_field_set_value(recipient, field_vbo_element, &mesh_data->vbo_element);
    mono_field_set_value(recipient, field_vertices_count, &mesh_data->vertices_count);
    mono_field_set_value(recipient, field_indices_count, &mesh_data->indices_count);
    mono_field_set_value(recipient, field_is_line, &mesh_data->is_line);

    delete mesh_data;
  }

  void
  CreateMeshMono(MonoArray *vertices_array, bool is_line, MonoObject *recipient) {
    i32 vertices_length = mono_array_length(vertices_array);
    std::vector<f32> vertices;
    for (i32 i = 0; i < vertices_length; ++i)
      vertices.push_back(mono_array_get(vertices_array, f32, i));

    MeshData *mesh_data = CreateMesh(vertices, is_line);

    MonoClass *klass                     = mono_object_get_class(recipient);
    MonoClassField *field_vao            = mono_class_get_field_from_name(klass, "vao");
    MonoClassField *field_vbo_array      = mono_class_get_field_from_name(klass, "vboArray");
    MonoClassField *field_vbo_element    = mono_class_get_field_from_name(klass, "vboElement");
    MonoClassField *field_vertices_count = mono_class_get_field_from_name(klass, "verticesCount");
    MonoClassField *field_indices_count  = mono_class_get_field_from_name(klass, "indicesCount");
    MonoClassField *field_is_line        = mono_class_get_field_from_name(klass, "isLine");

    mono_field_set_value(recipient, field_vao, &mesh_data->vao);
    mono_field_set_value(recipient, field_vbo_array, &mesh_data->vbo_array);
    mono_field_set_value(recipient, field_vbo_element, &mesh_data->vbo_element);
    mono_field_set_value(recipient, field_vertices_count, &mesh_data->vertices_count);
    mono_field_set_value(recipient, field_indices_count, &mesh_data->indices_count);
    mono_field_set_value(recipient, field_is_line, &mesh_data->is_line);

    delete mesh_data;
  }

  void
  DrawMeshMono(MonoObject *obj) {
    MeshData *mesh_data = ToMeshData(obj);
    DrawMesh(mesh_data);
    delete mesh_data;
  }

  void
  DestroyMeshMono(MonoObject *obj) {
    MeshData *mesh_data = ToMeshData(obj);
    DestroyMesh(mesh_data);
    delete mesh_data;
  }

  void
  PlayAudioMono(MonoString *path, bool loop) {
    std::string u8_path = mono_string_to_utf8(path);
    PlayAudio(u8_path, loop);
  }

  ImageData *
  ToImageData(MonoObject *obj) {
    ImageData *data  = new ImageData();
    MonoClass *klass = mono_object_get_class(obj);

    MonoClassField *field_id = mono_class_get_field_from_name(klass, "id");

    if (!field_id) {
      std::cerr << "obj is not a Texture\n";
      return data;
    }

    mono_field_get_value(obj, field_id, &data->texture);

    return data;
  }

  void
  CreateTextureMono(MonoString *image_path, MonoObject *recipient) {
    std::string u8_path = mono_string_to_utf8(image_path);

    ImageData *image = CreateImage(path + u8_path);

    MonoClass *klass         = mono_object_get_class(recipient);
    MonoClassField *field_id = mono_class_get_field_from_name(klass, "id");
    mono_field_set_value(recipient, field_id, &image->texture);

    delete image;
  }

  void
  DestroyTextureMono(MonoObject *obj) {
    ImageData *image = ToImageData(obj);
    DestroyImage(image);
    delete image;
  }

  void
  DrawTextureMono(MonoObject *obj) {
    ShaderData *shader = GetCurrentShader();
    i32 is_texture_loc = GetLocation(shader, "IsTexture", false);
    SetInt(shader, is_texture_loc, 1);
    ImageData *image = ToImageData(obj);
    DrawImage(image);
    delete image;
  }

  void
  DrawColorMono(MonoObject *obj) {
    ShaderData *shader = GetCurrentShader();
    i32 is_texture_loc = GetLocation(shader, "IsTexture");
    i32 color_loc      = GetLocation(shader, "Color", false);
    SetInt(shader, is_texture_loc, 0);
    v3 color = ToVec3(obj);
    SetVec3(shader, color_loc, color);
  }

  ShaderData *
  ToShaderData(MonoObject *obj) {
    ShaderData *data = new ShaderData();
    MonoClass *klass = mono_object_get_class(obj);

    MonoClassField *field_id = mono_class_get_field_from_name(klass, "program");

    if (!field_id) {
      std::cerr << "obj is not a Shader\n";
      return data;
    }

    mono_field_get_value(obj, field_id, &data->program);

    return data;
  }

  void
  CreateShaderMono(MonoString *shader_path, MonoObject *recipient) {
    std::string u8_path = mono_string_to_utf8(shader_path);

    ShaderData *shader = CreateShader(path + u8_path);

    MonoClass *klass         = mono_object_get_class(recipient);
    MonoClassField *field_id = mono_class_get_field_from_name(klass, "program");
    mono_field_set_value(recipient, field_id, &shader->program);

    delete shader;
  }

  void
  DestroyShaderMono(MonoObject *obj) {
    ShaderData *shader = ToShaderData(obj);
    DestroyShader(shader);
    delete shader;
  }

  void
  DrawShaderMono(MonoObject *obj) {
    ShaderData *shader = ToShaderData(obj);
    DrawShader(shader);
    delete shader;
  }

  i32
  ShaderGetLocation(MonoObject *obj, MonoString *name) {
    std::string u8_name = mono_string_to_utf8(name);
    ShaderData *shader  = ToShaderData(obj);
    i32 loc             = GetLocation(shader, u8_name);
    delete shader;
    return loc;
  }

  void
  ShaderSetInt(MonoObject *obj, i32 location, i32 val) {
    ShaderData *shader = ToShaderData(obj);
    SetInt(shader, location, val);
    delete shader;
  }

  void
  ShaderSetVec2(MonoObject *obj, i32 location, MonoObject *val) {
    ShaderData *shader = ToShaderData(obj);
    v2 vec             = ToVec2(val);
    SetVec2(shader, location, vec);
    delete shader;
  }

  void
  ShaderSetVec3(MonoObject *obj, i32 location, MonoObject *val) {
    ShaderData *shader = ToShaderData(obj);
    v3 vec             = ToVec3(val);
    SetVec3(shader, location, vec);
    delete shader;
  }

  void
  ShaderSetVec4(MonoObject *obj, i32 location, MonoObject *val) {
    ShaderData *shader = ToShaderData(obj);
    v4 vec             = ToVec4(val);
    SetVec4(shader, location, vec);
    delete shader;
  }

  void
  ShaderSetMat3(MonoObject *obj, i32 location, MonoObject *val) {
    ShaderData *shader = ToShaderData(obj);
    mat3 mat           = ToMat3(val);
    SetMat3(shader, location, mat);
    delete shader;
  }

  void
  ShaderSetMat4(MonoObject *obj, i32 location, MonoObject *val) {
    ShaderData *shader = ToShaderData(obj);
    mat4 mat           = ToMat4(val);
    SetMat4(shader, location, mat);
    delete shader;
  }

  void
  TranslateMono(MonoObject *obj) {
    v3 pos = ToVec3(obj);
    TranslateMesh(pos);
  }

  MonoObject *
  GetMousePosMono() {
    v2d pos = GetMousePos();
    v2 fpos;
    fpos.x          = (f32)pos.x;
    fpos.y          = (f32)pos.y;
    MonoObject *vec = FromVec2(fpos);
    return vec;
  }

  void
  RotateMono(MonoObject *obj) {
    v3 rotate = ToVec3(obj);
    RotateMesh(rotate);
  }

  void
  NewStash(MonoObject *recipient) {
    v3 position                    = GetPosition();
    quat rotation                  = GetRotation();
    MonoClass *klass               = mono_object_get_class(recipient);
    MonoClassField *field_position = mono_class_get_field_from_name(klass, "position");
    MonoClassField *field_rotation = mono_class_get_field_from_name(klass, "rotation");
    mono_field_set_value(recipient, field_position, FromVec3(position));
    mono_field_set_value(recipient, field_rotation, FromQuat(rotation));
  }

  void
  PopStash(MonoObject *stash) {
    MonoClass *klass               = mono_object_get_class(stash);
    MonoClassField *field_position = mono_class_get_field_from_name(klass, "position");
    MonoClassField *field_rotation = mono_class_get_field_from_name(klass, "rotation");

    if (!field_position || !field_rotation) { std::cerr << "obj is not a Stash\n"; }

    MonoObject *obj_position;
    MonoObject *obj_rotation;
    mono_field_get_value(stash, field_position, &obj_position);
    mono_field_get_value(stash, field_rotation, &obj_rotation);
    if (!obj_position) std::cout << "obj_position is null\n";
    if (!obj_rotation) std::cout << "obj_rotation is null\n";
    v3 position   = ToVec3(obj_position);
    quat rotation = ToQuat(obj_rotation);

    TranslateMesh(position);
    SetRotation(rotation);
  }

  void
  AddFeatures() {
    mono_add_internal_call("VisualKey.Window::CreateWindow", (const void *)CreateWindowMono);
    mono_add_internal_call("VisualKey.Window::DestroyWindow", (const void *)DestroyWindowMono);
    mono_add_internal_call("VisualKey.Window::BackgroundWindow", (const void *)ClearBackground);
    mono_add_internal_call("VisualKey.Window::SizeWindow", (const void *)WindowSize);
    mono_add_internal_call("VisualKey.Window::FullscreenWindow", (const void *)WindowFullscreen);
    mono_add_internal_call("VisualKey.Window::MakeCurrentWindow",
                           (const void *)MakeCurrentWindowMono);
    mono_add_internal_call("VisualKey.Window::SetOrtho", (const void *)SetOrtho);
    mono_add_internal_call("VisualKey.Window::GetOrtho", (const void *)IsOrtho);

    mono_add_internal_call("VisualKey.Time::DeltaTime", (const void *)DeltaTime);
    mono_add_internal_call("VisualKey.Time::GetTime", (const void *)glfwGetTime);

    mono_add_internal_call("VisualKey.Mesh::CreateMesh", (const void *)CreateMeshMono);
    mono_add_internal_call("VisualKey.Mesh::CreateMeshIndices",
                           (const void *)CreateMeshIndicesMono);
    mono_add_internal_call("VisualKey.Mesh::DestroyMesh", (const void *)DestroyMeshMono);
    mono_add_internal_call("VisualKey.Mesh::DrawMesh", (const void *)DrawMeshMono);
    mono_add_internal_call("VisualKey.Mesh::PolygonMode", (const void *)PolygonMode);
    mono_add_internal_call("VisualKey.Mesh::Translate", (const void *)TranslateMono);
    mono_add_internal_call("VisualKey.Mesh::Rotate", (const void *)RotateMono);
    mono_add_internal_call("VisualKey.Mesh::RotateX", (const void *)RotateXMesh);
    mono_add_internal_call("VisualKey.Mesh::RotateY", (const void *)RotateYMesh);
    mono_add_internal_call("VisualKey.Mesh::RotateZ", (const void *)RotateZMesh);

    mono_add_internal_call("VisualKey.Stash::NewStash", (const void *)NewStash);
    mono_add_internal_call("VisualKey.Stash::PopStash", (const void *)PopStash);

    mono_add_internal_call("VisualKey.Input::IsKeyPressed", (const void *)IsKeyPressed);
    mono_add_internal_call("VisualKey.Input::IsMousePressed", (const void *)IsMousePressed);
    mono_add_internal_call("VisualKey.Input::IsPadPressed", (const void *)IsPadPressed);
    mono_add_internal_call("VisualKey.Input::IsKeyJustPressed", (const void *)IsKeyJustPressed);
    mono_add_internal_call("VisualKey.Input::IsMouseJustPressed", (const void *)IsMouseJustPressed);
    mono_add_internal_call("VisualKey.Input::IsPadJustPressed", (const void *)IsPadJustPressed);
    mono_add_internal_call("VisualKey.Input::IsKeyJustReleased", (const void *)IsKeyJustReleased);
    mono_add_internal_call("VisualKey.Input::IsMouseJustReleased",
                           (const void *)IsMouseJustReleased);
    mono_add_internal_call("VisualKey.Input::IsPadJustReleased", (const void *)IsPadJustReleased);
    mono_add_internal_call("VisualKey.Input::GetJoyStick", (const void *)GetJoyStick);
    mono_add_internal_call("VisualKey.Input::GetMousePos", (const void *)GetMousePosMono);

    mono_add_internal_call("VisualKey.Audio::PlayAudio", (const void *)PlayAudioMono);

    mono_add_internal_call("VisualKey.Texture::CreateTexture", (const void *)CreateTextureMono);
    mono_add_internal_call("VisualKey.Texture::DestroyTexture", (const void *)DestroyTextureMono);
    mono_add_internal_call("VisualKey.Texture::DrawTexture", (const void *)DrawTextureMono);

    mono_add_internal_call("VisualKey.Color::DrawColor", (const void *)DrawColorMono);

    mono_add_internal_call("VisualKey.Shader::CreateShader", (const void *)CreateShaderMono);
    mono_add_internal_call("VisualKey.Shader::DestroyShader", (const void *)DestroyShaderMono);
    mono_add_internal_call("VisualKey.Shader::DrawShader", (const void *)DrawShaderMono);
    mono_add_internal_call("VisualKey.Shader::GetLocation", (const void *)ShaderGetLocation);
    mono_add_internal_call("VisualKey.Shader::SetInt", (const void *)ShaderSetInt);
    mono_add_internal_call("VisualKey.Shader::SetVec2", (const void *)ShaderSetVec2);
    mono_add_internal_call("VisualKey.Shader::SetVec3", (const void *)ShaderSetVec3);
    mono_add_internal_call("VisualKey.Shader::SetVec4", (const void *)ShaderSetVec4);
    mono_add_internal_call("VisualKey.Shader::SetMat3", (const void *)ShaderSetMat3);
    mono_add_internal_call("VisualKey.Shader::SetMat4", (const void *)ShaderSetMat4);
  }

}
