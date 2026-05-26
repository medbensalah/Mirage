#pragma once

#include "Mirage/Renderer/MeshOptimizer.h"

namespace Mirage
{
    class GLTFImporter
    {
    public:
        // Returns true when the file is parsed and output buffers are populated.
        static bool Import(const std::string& path, std::vector<MeshVertexPNT>& outVertices, std::vector<uint32_t>& outIndices);
    };
}

