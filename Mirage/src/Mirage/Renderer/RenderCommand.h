#pragma once

#include "Mirage/Renderer/RenderAPI.h"

namespace Mirage
{
    class RenderCommand
    {
    public:
        inline static void Init()
        {
            s_RenderAPI->Init();
        }

        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RenderAPI->SetViewport(x, y, width, height);
        }

        inline static void SetClearColor(const Vec4& color)
        {
            s_RenderAPI->SetClearColor(color);
        }
        
        inline static void Clear()
        {
            s_RenderAPI->Clear();
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
        {
            s_RenderAPI->DrawIndexed(vertexArray, indexCount);
        }
        inline static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0)
        {
            s_RenderAPI->DrawLines(vertexArray, vertexCount);
        }
        inline static void SetLineWidth(float width)
        {
            s_RenderAPI->SetineWidth(width);
        }

    private:
        static Scope<RenderAPI> s_RenderAPI;
    };
}
