#include "MrgPch.h"
#include "Mirage/Renderer/MeshOptimizer.h"

namespace Mirage
{
    struct VertexKey
    {
        int px, py, pz;
        int nx, ny, nz;
        int u, v;
        bool operator==(const VertexKey& other) const
        {
            return px == other.px && py == other.py && pz == other.pz &&
                   nx == other.nx && ny == other.ny && nz == other.nz &&
                   u == other.u && v == other.v;
        }
    };
    struct VertexKeyHasher
    {
        size_t operator()(const VertexKey& k) const
        {
            size_t h = 1469598103934665603ull;
            auto mix = [&](int x)
            {
                h ^= (size_t)(uint32_t)x;
                h *= 1099511628211ull;
            };
            mix(k.px); mix(k.py); mix(k.pz);
            mix(k.nx); mix(k.ny); mix(k.nz);
            mix(k.u); mix(k.v);
            return h;
        }
    };

    static int Q(float f) { return (int)std::round(f * 100000.0f); }

    void MeshOptimizer::Optimize(std::vector<MeshVertexPNT>& vertices, std::vector<uint32_t>& indices)
    {
        // Fallback optimization pass: weld exact/near-exact duplicate vertices.
        // This preserves behavior and reduces vertex bandwidth before GPU upload.
        std::unordered_map<VertexKey, uint32_t, VertexKeyHasher> remap;
        std::vector<MeshVertexPNT> outVertices;
        outVertices.reserve(vertices.size());

        std::vector<uint32_t> outIndices;
        outIndices.reserve(indices.size());

        for (uint32_t idx : indices)
        {
            const auto& v = vertices[idx];
            VertexKey key{Q(v.Position.x), Q(v.Position.y), Q(v.Position.z),
                          Q(v.Normal.x), Q(v.Normal.y), Q(v.Normal.z),
                          Q(v.UV.x), Q(v.UV.y)};
            auto it = remap.find(key);
            if (it == remap.end())
            {
                uint32_t newIndex = (uint32_t)outVertices.size();
                outVertices.push_back(v);
                remap[key] = newIndex;
                outIndices.push_back(newIndex);
            }
            else
            {
                outIndices.push_back(it->second);
            }
        }

        vertices.swap(outVertices);
        indices.swap(outIndices);
    }
}
