#include "MrgPch.h"

#include "Mirage/Renderer/Renderer3D.h"
#include "Mirage/Renderer/RenderCommand.h"
#include "Mirage/Renderer/Buffer.h"
#include "Mirage/Renderer/MeshOptimizer.h"
#include "Mirage/Renderer/UniformBuffer.h"
#include "Mirage/Renderer/ModelImporter.h"

namespace Mirage::Renderer3D
{
    static constexpr int kMaxPointLights = 32;
    struct MeshVertex
    {
        Vec3 Position;
        Vec3 Normal;
        Vec2 UV;
    };

    struct Renderer3DData
    {
        Ref<VertexArray> CubeVertexArray;
        Ref<VertexBuffer> CubeVertexBuffer;
        Ref<IndexBuffer> CubeIndexBuffer;
        Ref<VertexArray> SphereVertexArray;
        Ref<VertexBuffer> SphereVertexBuffer;
        Ref<IndexBuffer> SphereIndexBuffer;
        Ref<Shader> PBRShader;
        std::unordered_map<std::string, Ref<VertexArray>> MeshCache;
        std::unordered_map<std::string, std::pair<Vec3, Vec3>> MeshBoundsCache;

        Mat4 ViewProjection = Mat4(1.0f);
        Vec3 CameraPosition = { 0.0f, 0.0f, 0.0f };
        DirectionalLightData MainLight;
        std::vector<PointLightData> PointLights;
        Ref<UniformBuffer> CameraUBO;
        Ref<UniformBuffer> ObjectUBO;
        Ref<UniformBuffer> LightingUBO;
        int OutlinedEntity = -1;
    };

    struct CameraUBOData
    {
        Mat4 ViewProjection;
        Vec4 CameraPos;
    };

    struct ObjectUBOData
    {
        Mat4 Transform;
        Mat4 NormalMatrix;
        Vec4 AlbedoMetallic;
        Vec4 RoughnessAoPad;
        VecI4 EntityIDPad;
    };

    struct GPUDirectionalLight
    {
        Vec4 DirectionIntensity;
        Vec4 ColorPad;
    };
    struct GPUPointLight
    {
        Vec4 PositionRadius;
        Vec4 ColorIntensity;
    };
    struct LightingUBOData
    {
        GPUDirectionalLight Light;
        VecI4 PointLightCountPad;
        GPUPointLight PointLights[kMaxPointLights];
        Vec4 IBLDiffuse;
        Vec4 IBLSpecular;
    };

    static Renderer3DData s_Data;

    static void BuildCube()
    {
        std::vector<MeshVertexPNT> cubeVertices = {
            // Front
            {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},
            // Back
            {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
            {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},
            // Left
            {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
            {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
            {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
            // Right
            {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
            {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
            // Top
            {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
            // Bottom
            {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
            {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},
        };

        std::vector<uint32_t> cubeIndices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20
        };

        MeshOptimizer::Optimize(cubeVertices, cubeIndices);

        s_Data.CubeVertexArray = VertexArray::Create();
        s_Data.CubeVertexBuffer = VertexBuffer::Create((float*)cubeVertices.data(), (uint32_t)(cubeVertices.size() * sizeof(MeshVertexPNT)));
        s_Data.CubeVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);
        s_Data.CubeIndexBuffer = IndexBuffer::Create(cubeIndices.data(), (uint32_t)cubeIndices.size());
        s_Data.CubeVertexArray->SetIndexBuffer(s_Data.CubeIndexBuffer);
        s_Data.MeshBoundsCache["__builtin_cube__"] = {Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.5f, 0.5f, 0.5f)};
    }

    static void BuildSphere()
    {
        const uint32_t stacks = 24;
        const uint32_t slices = 32;
        std::vector<MeshVertexPNT> vertices;
        std::vector<uint32_t> indices;
        vertices.reserve((stacks + 1) * (slices + 1));
        indices.reserve(stacks * slices * 6);

        for (uint32_t y = 0; y <= stacks; y++)
        {
            float v = (float)y / (float)stacks;
            float phi = v * glm::pi<float>();
            for (uint32_t x = 0; x <= slices; x++)
            {
                float u = (float)x / (float)slices;
                float theta = u * glm::two_pi<float>();
                Vec3 n = {
                    sin(phi) * cos(theta),
                    cos(phi),
                    sin(phi) * sin(theta)
                };
                MeshVertexPNT vert{};
                vert.Position = n * 0.5f;
                vert.Normal = glm::normalize(n);
                vert.UV = {u, 1.0f - v};
                vertices.push_back(vert);
            }
        }

        for (uint32_t y = 0; y < stacks; y++)
        {
            for (uint32_t x = 0; x < slices; x++)
            {
                uint32_t i0 = y * (slices + 1) + x;
                uint32_t i1 = i0 + 1;
                uint32_t i2 = i0 + (slices + 1);
                uint32_t i3 = i2 + 1;
                indices.push_back(i0); indices.push_back(i2); indices.push_back(i1);
                indices.push_back(i1); indices.push_back(i2); indices.push_back(i3);
            }
        }

        s_Data.SphereVertexArray = VertexArray::Create();
        s_Data.SphereVertexBuffer = VertexBuffer::Create((float*)vertices.data(), (uint32_t)(vertices.size() * sizeof(MeshVertexPNT)));
        s_Data.SphereVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        s_Data.SphereVertexArray->AddVertexBuffer(s_Data.SphereVertexBuffer);
        s_Data.SphereIndexBuffer = IndexBuffer::Create(indices.data(), (uint32_t)indices.size());
        s_Data.SphereVertexArray->SetIndexBuffer(s_Data.SphereIndexBuffer);
        s_Data.MeshBoundsCache["__builtin_sphere__"] = {Vec3(-0.5f), Vec3(0.5f)};
    }

    void Init()
    {
        BuildCube();
        BuildSphere();
        s_Data.PBRShader = Shader::Create("assets/shaders/PBRMesh.glsl");
        s_Data.CameraUBO = UniformBuffer::Create(sizeof(CameraUBOData), 0);
        s_Data.ObjectUBO = UniformBuffer::Create(sizeof(ObjectUBOData), 1);
        s_Data.LightingUBO = UniformBuffer::Create(sizeof(LightingUBOData), 2);
    }

    void Shutdown()
    {
    }

    void BeginScene(const Camera& camera, const Mat4& cameraTransform)
    {
        s_Data.ViewProjection = camera.GetProjection() * Inverse(cameraTransform);
        s_Data.CameraPosition = Vec3(cameraTransform[3]);
        CameraUBOData cam{};
        cam.ViewProjection = s_Data.ViewProjection;
        cam.CameraPos = Vec4(s_Data.CameraPosition, 1.0f);
        s_Data.CameraUBO->SetData(&cam, sizeof(cam));
    }

    void BeginScene(const EditorCamera& camera)
    {
        s_Data.ViewProjection = camera.GetViewProjection();
        s_Data.CameraPosition = camera.GetPosition();
        CameraUBOData cam{};
        cam.ViewProjection = s_Data.ViewProjection;
        cam.CameraPos = Vec4(s_Data.CameraPosition, 1.0f);
        s_Data.CameraUBO->SetData(&cam, sizeof(cam));
    }

    void EndScene()
    {
    }

    void SetDirectionalLight(const DirectionalLightData& light)
    {
        s_Data.MainLight = light;
    }

    void SetPointLights(const std::vector<PointLightData>& lights)
    {
        s_Data.PointLights = lights;
        if (s_Data.PointLights.size() > kMaxPointLights)
            s_Data.PointLights.resize(kMaxPointLights);
    }

    void SetOutlinedEntity(int entityID)
    {
        s_Data.OutlinedEntity = entityID;
    }

    static Ref<VertexArray> LoadMeshFromPath(const std::string& path, bool optimize)
    {
        auto cached = s_Data.MeshCache.find(path);
        if (cached != s_Data.MeshCache.end())
            return cached->second;

        std::vector<MeshVertexPNT> vertices;
        std::vector<uint32_t> indices;
        if (!ModelImporter::Import(path, vertices, indices))
            return nullptr;
        if (optimize)
            MeshOptimizer::Optimize(vertices, indices);

        Vec3 mn(FLT_MAX), mx(-FLT_MAX);
        for (const auto& v : vertices)
        {
            mn = glm::min(mn, v.Position);
            mx = glm::max(mx, v.Position);
        }

        auto va = VertexArray::Create();
        auto vb = VertexBuffer::Create((float*)vertices.data(), (uint32_t)(vertices.size() * sizeof(MeshVertexPNT)));
        vb->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        va->AddVertexBuffer(vb);
        va->SetIndexBuffer(IndexBuffer::Create(indices.data(), (uint32_t)indices.size()));
        s_Data.MeshCache[path] = va;
        s_Data.MeshBoundsCache[path] = {mn, mx};
        return va;
    }

    void DrawMesh(const MeshComponent& mesh, const Mat4& transform, const MeshRendererComponent& material, int entityID)
    {
        s_Data.PBRShader->Bind();
        ObjectUBOData obj{};
        obj.Transform = transform;
        obj.NormalMatrix = glm::transpose(glm::inverse(transform));
        obj.AlbedoMetallic = Vec4(material.AlbedoColor.r, material.AlbedoColor.g, material.AlbedoColor.b, material.Metallic);
        obj.RoughnessAoPad = Vec4(material.Roughness, material.AO, 0.0f, 0.0f);
        obj.EntityIDPad = VecI4(entityID, 0, 0, 0);
        s_Data.ObjectUBO->SetData(&obj, sizeof(obj));

        LightingUBOData light{};
        light.Light.DirectionIntensity = Vec4(s_Data.MainLight.Direction, s_Data.MainLight.Intensity);
        light.Light.ColorPad = Vec4(s_Data.MainLight.Color, 0.0f);
        light.PointLightCountPad = VecI4((int)s_Data.PointLights.size(), 0, 0, 0);
        for (int i = 0; i < (int)s_Data.PointLights.size(); i++)
        {
            const auto& pl = s_Data.PointLights[i];
            light.PointLights[i].PositionRadius = Vec4(pl.Position, pl.Radius);
            light.PointLights[i].ColorIntensity = Vec4(pl.Color, pl.Intensity);
        }
        // Keep a stronger default ambient term so editor meshes don't appear black
        // when they are not directly facing the key light.
        light.IBLDiffuse = Vec4(0.18f, 0.18f, 0.18f, 0.0f);
        light.IBLSpecular = Vec4(0.08f, 0.08f, 0.08f, 0.0f);
        s_Data.LightingUBO->SetData(&light, sizeof(light));

        Ref<VertexArray> meshVA = (mesh.PrimitiveType == MeshComponent::Primitive::Sphere) ? s_Data.SphereVertexArray : s_Data.CubeVertexArray;
        if (!mesh.SourcePath.empty())
        {
            Ref<VertexArray> imported = LoadMeshFromPath(mesh.SourcePath, mesh.OptimizeOnLoad);
            if (imported)
                meshVA = imported;
        }
        RenderCommand::DrawIndexed(meshVA);

    }

    bool TryGetMeshLocalBounds(const ::Mirage::MeshComponent& mesh, Vec3& outMin, Vec3& outMax)
    {
        if (mesh.SourcePath.empty())
        {
            auto it = s_Data.MeshBoundsCache.find(mesh.PrimitiveType == MeshComponent::Primitive::Sphere ? "__builtin_sphere__" : "__builtin_cube__");
            if (it == s_Data.MeshBoundsCache.end())
                return false;
            outMin = it->second.first;
            outMax = it->second.second;
            return true;
        }

        auto it = s_Data.MeshBoundsCache.find(mesh.SourcePath);
        if (it != s_Data.MeshBoundsCache.end())
        {
            outMin = it->second.first;
            outMax = it->second.second;
            return true;
        }

        Ref<VertexArray> va = LoadMeshFromPath(mesh.SourcePath, mesh.OptimizeOnLoad);
        if (!va)
            return false;
        it = s_Data.MeshBoundsCache.find(mesh.SourcePath);
        if (it == s_Data.MeshBoundsCache.end())
            return false;
        outMin = it->second.first;
        outMax = it->second.second;
        return true;
    }
}
