#pragma once

#include "Mirage/Renderer/RenderAPI.h"

namespace Mirage
{
    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    };
}
