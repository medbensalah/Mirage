#include <MrgPch.h>

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Mirage
{
    Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "No graphics API selected!");
            return nullptr;

        case RenderAPI::API::OpenGL:
            return new OpenGLShader(vertexSource, fragmentSource);
        }

        MRG_CORE_ERROR("Unknown graphics API!");
        MRG_CORE_ASSERT(false, "Unknown graphics API!");
        return nullptr;
    }
}
