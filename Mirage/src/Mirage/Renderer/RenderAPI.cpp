#include "MrgPch.h"

#include "Mirage/Renderer/RenderAPI.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Mirage
{
    RenderAPI::API RenderAPI::s_RenderAPI = RenderAPI::API::OpenGL;
    Scope<RenderAPI> RenderAPI::Create()
    {
        switch (s_RenderAPI)
        {
        case RenderAPI::API::None:    MRG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RenderAPI::API::OpenGL:  return CreateScope<OpenGLRenderAPI>();
        }

        MRG_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
