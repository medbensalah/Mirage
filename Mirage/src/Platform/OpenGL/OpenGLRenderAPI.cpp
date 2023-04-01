#include "MrgPch.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

#include <glad/glad.h>

namespace Mirage
{
    void OpenGLMessageCallback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char* message,
        const void* userParam)
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:            MRG_CORE_CRITICAL(message); return;
            case GL_DEBUG_SEVERITY_MEDIUM:          MRG_CORE_ERROR(message); return;
            case GL_DEBUG_SEVERITY_LOW:             MRG_CORE_WARN(message); return;
            case GL_DEBUG_SEVERITY_NOTIFICATION:    MRG_CORE_TRACE(message); return;
        }

        MRG_CORE_ASSERT(false, "Unknown severity level!");
    }
    
    void OpenGLRenderAPI::Init()
    {
        MRG_PROFILE_FUNCTION();

#ifdef MRG_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    	glEnable(GL_LINE_SMOOTH);
    }

    void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
    {
    	vertexArray->Bind();
        count = count ? count : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
    {
    	vertexArray->Bind();
        glDrawArrays(GL_LINES, 0, vertexCount);
    }

    void OpenGLRenderAPI::SetineWidth(float width)
    {
    	glLineWidth(width);
    }
}
