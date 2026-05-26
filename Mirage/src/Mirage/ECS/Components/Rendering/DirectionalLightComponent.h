#pragma once

#include "Mirage/Math/glmTypes.h"

namespace Mirage
{
    struct DirectionalLightComponent
    {
        Vec3 Direction = { -0.4f, -1.0f, -0.3f };
        Vec3 Color = { 1.0f, 1.0f, 1.0f };
        float Intensity = 5.0f;
    };

    void RegisterDirectionalLightGizmoDrawer();
}
