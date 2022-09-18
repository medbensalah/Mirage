#pragma once

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Mirage
{
    MIRAGE_CLASS Renderer
    {
    public:
        static void Init();

        static void OnWindowResize(Vec2 size);
        
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();
        
        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const Mat4& transform = Mat4(1.0f));
        
        inline static RenderAPI::API GetGfxApi() { return RenderAPI::GetAPI(); }

    private:
        struct SceneData
        {
            Mat4 ViewProjectionMatrix;
        };

        static SceneData* s_SceneData;
    };
}
