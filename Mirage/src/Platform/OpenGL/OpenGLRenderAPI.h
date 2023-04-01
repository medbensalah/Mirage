#pragma once

#include "Mirage/Renderer/RenderAPI.h"

namespace Mirage
{
    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        virtual void Init() override;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) override;
        virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
        virtual void SetineWidth(float width) override;
    };
}
