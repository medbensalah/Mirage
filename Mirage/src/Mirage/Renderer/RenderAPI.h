#pragma once

#include "Mirage/Renderer/VertexArray.h"
#include "glm/vec4.hpp"
#include "Mirage/Core/Math.h"

namespace Mirage
{
    class RenderAPI
    {
    public:
        virtual ~RenderAPI() = default;

        enum class API
        {
            None = 0,
            OpenGL = 1
        };

        virtual void Init() = 0;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        
        virtual void SetClearColor(const Vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) = 0;

        inline static API GetAPI() { return s_RenderAPI; }
        static Scope<RenderAPI> Create();
        
        private:
        static API s_RenderAPI;
    };
}
