#pragma once

#include "Mirage/Math/glmTypes.h"

namespace Mirage
{
    struct MeshVertexPNT
    {
        Vec3 Position;
        Vec3 Normal;
        Vec2 UV;
    };

    class MeshOptimizer
    {
    public:
        static void Optimize(std::vector<MeshVertexPNT>& vertices, std::vector<uint32_t>& indices);
    };
}

