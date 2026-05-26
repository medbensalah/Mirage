#pragma once

#include "Mirage/Math/glmTypes.h"

namespace Mirage
{
    struct PointLightComponent
    {
        Vec3 Color = { 1.0f, 1.0f, 1.0f };
        float Intensity = 20.0f;
        float Radius = 10.0f;
    };

    void RegisterPointLightGizmoDrawer();
}
