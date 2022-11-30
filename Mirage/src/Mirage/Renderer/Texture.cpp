#include "MrgPch.h"

#include "Mirage/Renderer/Texture.h"

#include "Mirage/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"


namespace Mirage
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "No graphics API selected!");
            return nullptr;
            
        case RenderAPI::API::OpenGL:
            return CreateRef<OpenGLTexture2D>(width, height); 
        }

        MRG_CORE_ASSERT(false, "Unknown graphics API!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::filesystem::path& path)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "No graphics API selected!");
            return nullptr;
            
        case RenderAPI::API::OpenGL:
            return CreateRef<OpenGLTexture2D>(path); 
        }

        MRG_CORE_ASSERT(false, "Unknown graphics API!");
        return nullptr;
    }
}
