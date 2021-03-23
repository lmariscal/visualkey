#include "light.h"
#include "shader.h"

namespace visualkey {

  bool light_system = false;

  void
  EnableLightSystem() {
    light_system          = true;
    ShaderData *shader    = GetCurrentShader();
    i32 ambient_light_loc = GetLocation(shader, "AmbientLight");
    SetFloat(shader, ambient_light_loc, 0.1f);
  }

  void
  DisableLightSystem() {
    light_system          = true;
    ShaderData *shader    = GetCurrentShader();
    i32 ambient_light_loc = GetLocation(shader, "AmbientLight");
    SetFloat(shader, ambient_light_loc, 1.0f);
  }

}
