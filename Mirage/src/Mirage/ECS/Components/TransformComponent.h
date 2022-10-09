#pragma once
#include "Mirage/Core/Math.h"

namespace Mirage
{
    struct TransformComponent
    {
        Mat4 Trannsform = Mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const Mat4& transform)
            : Trannsform(transform)
        {
        }

        operator const Mat4&() const { return Trannsform; }
        operator Mat4&() { return Trannsform; }
    };
}
