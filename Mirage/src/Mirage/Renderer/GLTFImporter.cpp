#include "MrgPch.h"
#include "Mirage/Renderer/GLTFImporter.h"

namespace Mirage
{
    bool GLTFImporter::Import(const std::string& path, std::vector<MeshVertexPNT>& outVertices, std::vector<uint32_t>& outIndices)
    {
        // High-performance pipeline hook:
        // - cgltf parsing
        // - meshoptimizer remap/optimize
        // - optional KTX2/Basis texture path
        // This repository currently does not ship cgltf, so we fail fast with diagnostics.
        (void)path;
        outVertices.clear();
        outIndices.clear();
        MRG_CORE_ERROR("GLTF importer is not active: cgltf is not vendored yet.");
        return false;
    }
}

