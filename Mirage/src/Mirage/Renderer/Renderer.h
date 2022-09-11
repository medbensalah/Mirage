#pragma once

#include "RenderCommand.h"

namespace Mirage
{
    MIRAGE_CLASS Renderer
    {
    public:
        static void BeginScene();
        static void EndScene();
        
        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
        
        inline static RenderAPI::API GetGfxApi() { return RenderAPI::GetAPI(); }
    };
}
