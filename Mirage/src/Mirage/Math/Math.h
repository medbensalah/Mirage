#pragma once

#include <glm/glm.hpp>

#include "Mirage/Math//GLMTypes.h"

namespace Mirage
{
    namespace Math
    {
        bool DecomposeTransform(const Mat4& transform, Vec3& position, Vec3& rotation, Vec3& scale);
    }
}
