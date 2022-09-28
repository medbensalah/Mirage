#pragma once

#include <glm/glm.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Mirage
{
#define VecI2       glm::ivec2
#define VecI3       glm::ivec3
#define VecI4       glm::ivec4

#define Vec2        glm::fvec2
#define Vec3        glm::fvec3
#define Vec4        glm::fvec4

#define MatI2       glm::imat2
#define MatI2       glm::imat2
#define MatI3       glm::imat3

#define Mat2        glm::mat2
#define Mat3        glm::mat3
#define Mat4        glm::mat4

#define Invert      glm::inverse

#define MatTranslate   glm::translate
#define MatRotate      glm::rotate
#define MatScale      glm::scale

#define Radians    glm::radians
#define Degrees    glm::degrees
}
