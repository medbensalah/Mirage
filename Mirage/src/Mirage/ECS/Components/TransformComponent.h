#pragma once
#include "Mirage/Core/Math.h"

namespace Mirage
{
    struct TransformComponent
    {
        Mat4 Transform = Mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const Mat4& transform)
            : Transform(transform)
        {
        }

        operator const Mat4&() const { return Transform; }
        operator Mat4&() { return Transform; }
    };
}
