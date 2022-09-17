#pragma once
#include "VertexArray.h"
#include "glm/vec4.hpp"

namespace Mirage
{
    MIRAGE_CLASS RenderAPI
    {
    public:
        virtual ~RenderAPI() = default;

        enum class API
        {
            None = 0,
            OpenGL = 1
        };

        virtual void Init() = 0;
        
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

        inline static API GetAPI() { return s_RenderAPI; }

        private:
        static API s_RenderAPI;
    };
}
