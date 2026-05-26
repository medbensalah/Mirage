#pragma once

#include "Mirage/Math/glmTypes.h"
#include "Mirage/Renderer/Camera.h"
#include "Mirage/Renderer/EditorCamera.h"
#include "Mirage/Renderer/VertexArray.h"
#include "Mirage/Renderer/Shader.h"
#include "Mirage/ECS/Components/Rendering/MeshComponent.h"
#include "Mirage/ECS/Components/Rendering/MeshRendererComponent.h"
#include <vector>

namespace Mirage::Renderer3D
{
    struct DirectionalLightData
    {
        Vec3 Direction = { -0.4f, -1.0f, -0.3f };
        Vec3 Color = { 1.0f, 1.0f, 1.0f };
        float Intensity = 5.0f;
    };
    struct PointLightData
    {
        Vec3 Position = { 0.0f, 0.0f, 0.0f };
        Vec3 Color = { 1.0f, 1.0f, 1.0f };
        float Intensity = 20.0f;
        float Radius = 10.0f;
    };

    void Init();
    void Shutdown();

    void BeginScene(const Camera& camera, const Mat4& cameraTransform);
    void BeginScene(const EditorCamera& camera);
    void EndScene();

    void SetDirectionalLight(const DirectionalLightData& light);
    void SetPointLights(const std::vector<PointLightData>& lights);
    void SetOutlinedEntity(int entityID);
    void DrawMesh(const class MeshComponent& mesh, const Mat4& transform, const MeshRendererComponent& material, int entityID = -1);
    bool TryGetMeshLocalBounds(const ::Mirage::MeshComponent& mesh, Vec3& outMin, Vec3& outMax);
}
