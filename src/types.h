#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>

namespace visualkey {

using namespace glm;

using i8    = signed char ;
using i16   = signed short;
using i32   = signed int;
using i64   = signed long long;

using u8    = unsigned char;
using u16   = unsigned short;
using u32   = unsigned int;
using u64   = unsigned long long;

using f32   = float ;
using f64   = double;

using v2 = vec2;
using v3 = vec3;
using v4 = vec4;

using v2i = vec<2, i32>;
using v2d = vec<2, f64>;

using m2 = mat2;
using m3 = mat3;
using m4 = mat4;

using uchar = unsigned char;

}
