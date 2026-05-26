#pragma once

#include "Mirage/Renderer/MeshOptimizer.h"

namespace Mirage
{
    class ModelImporter
    {
    public:
        static bool Import(const std::string& path, std::vector<MeshVertexPNT>& outVertices, std::vector<uint32_t>& outIndices);
    };
}

