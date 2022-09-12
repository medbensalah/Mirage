#pragma once

#include <glm/glm.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Mirage
{
#define Vec2        glm::vec2
#define Vec3        glm::vec3
#define Vec4        glm::vec4

#define Mat2        glm::mat2
#define Mat3        glm::mat3
#define Mat4        glm::mat4

#define Invert      glm::inverse

#define Translate   glm::translate
#define Rotate      glm::rotate

#define Radians    glm::radians
}
