#include "MrgPch.h"
#include "Mirage/Renderer/Framebuffer.h"

#include "Mirage/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Mirage
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecs& specs)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RenderAPI::API::OpenGL:
            return CreateRef<OpenGLFramebuffer>(specs);
        }

        MRG_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
