#include "MrgPch.h"

#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"


namespace Mirage
{
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "No graphics API selected!");
            return nullptr;
            
        case RenderAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path); 
        }

        MRG_CORE_ERROR("Unknown graphics API!");
        MRG_CORE_ASSERT(false, "Unknown graphics API!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::GetDefaultTexture()
    {
        static Ref<Texture2D> whiteTexture = Texture2D::Create("assets/textures/Default.png");
        return whiteTexture;
    }
}
