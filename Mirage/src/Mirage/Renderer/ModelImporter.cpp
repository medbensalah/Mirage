#include "MrgPch.h"
#include "Mirage/Renderer/ModelImporter.h"

#include <sstream>

namespace Mirage
{
    static bool ImportOBJ(const std::string& path, std::vector<MeshVertexPNT>& outVertices, std::vector<uint32_t>& outIndices)
    {
        std::ifstream in(path);
        if (!in.is_open())
            return false;

        std::vector<Vec3> positions;
        std::vector<Vec3> normals;
        std::vector<Vec2> uvs;
        std::unordered_map<std::string, uint32_t> remap;

        std::string line;
        while (std::getline(in, line))
        {
            if (line.size() < 2) continue;
            std::istringstream ss(line);
            std::string tag;
            ss >> tag;
            if (tag == "v")
            {
                Vec3 p{};
                ss >> p.x >> p.y >> p.z;
                positions.push_back(p);
            }
            else if (tag == "vn")
            {
                Vec3 n{};
                ss >> n.x >> n.y >> n.z;
                normals.push_back(n);
            }
            else if (tag == "vt")
            {
                Vec2 t{};
                ss >> t.x >> t.y;
                t.y = 1.0f - t.y;
                uvs.push_back(t);
            }
            else if (tag == "f")
            {
                std::vector<std::string> verts;
                std::string tok;
                while (ss >> tok) verts.push_back(tok);
                if (verts.size() < 3) continue;

                auto resolveIndex = [](int idx, int count) -> int
                {
                    if (idx > 0) return idx - 1;
                    if (idx < 0) return count + idx;
                    return -1;
                };

                auto emitVert = [&](const std::string& key)
                {
                    auto it = remap.find(key);
                    if (it != remap.end())
                    {
                        outIndices.push_back(it->second);
                        return;
                    }

                    int pi = 0, ti = 0, ni = 0;
                    {
                        std::vector<std::string> parts;
                        std::stringstream ks(key);
                        std::string part;
                        while (std::getline(ks, part, '/')) parts.push_back(part);
                        if (parts.size() > 0 && !parts[0].empty()) pi = std::stoi(parts[0]);
                        if (parts.size() > 1 && !parts[1].empty()) ti = std::stoi(parts[1]);
                        if (parts.size() > 2 && !parts[2].empty()) ni = std::stoi(parts[2]);
                    }
                    MeshVertexPNT v{};
                    int pidx = resolveIndex(pi, (int)positions.size());
                    int nidx = resolveIndex(ni, (int)normals.size());
                    int tidx = resolveIndex(ti, (int)uvs.size());
                    if (pidx >= 0 && pidx < (int)positions.size()) v.Position = positions[(size_t)pidx];
                    if (nidx >= 0 && nidx < (int)normals.size()) v.Normal = normals[(size_t)nidx];
                    if (tidx >= 0 && tidx < (int)uvs.size()) v.UV = uvs[(size_t)tidx];
                    uint32_t newIndex = (uint32_t)outVertices.size();
                    outVertices.push_back(v);
                    remap[key] = newIndex;
                    outIndices.push_back(newIndex);
                };

                // fan triangulation
                for (size_t i = 1; i + 1 < verts.size(); i++)
                {
                    emitVert(verts[0]);
                    emitVert(verts[i]);
                    emitVert(verts[i + 1]);
                }
            }
        }

        // If normals were missing, build smooth normals.
        bool hasNormals = false;
        for (const auto& v : outVertices)
        {
            if (glm::length(v.Normal) > 0.0001f) { hasNormals = true; break; }
        }
        if (!hasNormals)
        {
            std::vector<Vec3> acc(outVertices.size(), Vec3(0.0f));
            for (size_t i = 0; i + 2 < outIndices.size(); i += 3)
            {
                uint32_t i0 = outIndices[i], i1 = outIndices[i + 1], i2 = outIndices[i + 2];
                Vec3 p0 = outVertices[i0].Position;
                Vec3 p1 = outVertices[i1].Position;
                Vec3 p2 = outVertices[i2].Position;
                Vec3 n = glm::cross(p1 - p0, p2 - p0);
                if (glm::length(n) > 0.000001f)
                    n = glm::normalize(n);
                acc[i0] += n; acc[i1] += n; acc[i2] += n;
            }
            for (size_t i = 0; i < outVertices.size(); i++)
            {
                if (glm::length(acc[i]) > 0.000001f)
                    outVertices[i].Normal = glm::normalize(acc[i]);
                else
                    outVertices[i].Normal = Vec3(0.0f, 1.0f, 0.0f);
            }
        }

        return !outVertices.empty() && !outIndices.empty();
    }

    bool ModelImporter::Import(const std::string& path, std::vector<MeshVertexPNT>& outVertices, std::vector<uint32_t>& outIndices)
    {
        outVertices.clear();
        outIndices.clear();
        std::filesystem::path p(path);
        auto ext = p.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return (char)std::tolower(c); });

        if (ext == ".obj")
            return ImportOBJ(path, outVertices, outIndices);

        if (ext == ".gltf" || ext == ".glb" || ext == ".fbx")
        {
            MRG_CORE_WARN("Importer for {0} is not bundled in this repository build. Add Assimp/cgltf backend.", ext);
            return false;
        }

        return false;
    }
}
