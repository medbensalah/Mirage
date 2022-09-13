#pragma once

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Mirage
{
    MIRAGE_CLASS Renderer
    {
    public:
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();
        
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const Mat4& transform = Mat4(1.0f));
        
        inline static RenderAPI::API GetGfxApi() { return RenderAPI::GetAPI(); }

    private:
        struct SceneData
        {
            Mat4 ViewProjectionMatrix;
        };

        static SceneData* s_SceneData;
    };
}
