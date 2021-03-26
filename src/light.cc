#include "light.h"
#include "shader.h"

#include <map>
#include <iostream>

namespace visualkey {

  // Yep, next time plan an ECS system...
  std::map<u32, vec3> light_sources;
  std::vector<u32> dropped_keys;
  u32 ids             = 0;
  bool lights_enabled = false;

  void
  EnableLightSystem() {
    if (lights_enabled) return;
    ShaderData *shader           = GetCurrentShader();
    i32 number_light_sources_loc = GetLocation(shader, "NumberLightSources");
    i32 light_sources_loc        = GetLocation(shader, "LightSources");
    i32 ambient_light_loc        = GetLocation(shader, "AmbientLight");

    SetFloat(shader, ambient_light_loc, 0.21f);
    SetInt(shader, number_light_sources_loc, light_sources.size());

    std::vector<f32> lights(light_sources.size() * 3);
    i32 i = 0;
    for (auto const &[key, val] : light_sources) {
      lights.at(i)     = val[0];
      lights.at(i + 1) = val[1];
      lights.at(i + 2) = val[2];
      i++;
      if (i >= 255) { std::cerr << "Maximum number of Lights in the Uber Shader is 256\n"; }
    }

    SetVec3A(shader, light_sources_loc, lights);
    lights_enabled = true;
  }

  void
  DisableLightSystem() {
    if (!lights_enabled) return;
    ShaderData *shader           = GetCurrentShader();
    i32 number_light_sources_loc = GetLocation(shader, "NumberLightSources");
    i32 ambient_light_loc        = GetLocation(shader, "AmbientLight");

    SetFloat(shader, ambient_light_loc, 1.0f);
    SetInt(shader, number_light_sources_loc, 0);
    lights_enabled = false;
  }

  void
  DestroyLightSource(u32 id) {
    light_sources.erase(id);
    dropped_keys.push_back(id);

#ifdef VISUALKEY_DEBUG
    std::cout << "Destroyed Light Source: " << id << '\n';
#endif
  }

  u32
  CreateLightSource(v3 position) {
    u32 id = 0;
    if (dropped_keys.size() > 0) {
      id = dropped_keys[dropped_keys.size() - 1];
      dropped_keys.pop_back();
    } else {
      id = ids++;
    }
    light_sources.insert(std::pair(id, position));
#ifdef VISUALKEY_DEBUG
    std::cout << "Created Light Source: " << id << '\n';
#endif
    return id;
  }

  void
  ChangeLightPosition(u32 id, v3 new_pos) {
    if (light_sources.find(id) == light_sources.end()) return;
    light_sources[id] = new_pos;
  }

}
