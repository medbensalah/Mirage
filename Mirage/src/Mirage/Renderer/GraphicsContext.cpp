#include "MrgPch.h"
#include "Mirage/Renderer/GraphicsContext.h"

#include "Mirage/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Mirage {

    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
            
        case RenderAPI::API::OpenGL:
            return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        MRG_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}