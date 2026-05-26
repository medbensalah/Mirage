#pragma once

#include "Mirage/Math/glmTypes.h"

namespace Mirage
{
    struct MeshRendererComponent
    {
        Vec4 AlbedoColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        float Metallic = 0.1f;
        float Roughness = 0.6f;
        float AO = 1.0f;
    };
}

