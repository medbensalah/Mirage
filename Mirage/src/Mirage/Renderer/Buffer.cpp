#include <MrgPch.h>

#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Mirage
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                          ///
    ///                                       Buffer Layout                                      ///
    ///                                                                                          ///
    ////////////////////////////////////////////////////////////////////////////////////////////////




    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                          ///
    ///                                       Vertex Buffer                                      ///
    ///                                                                                          ///
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "No graphics API selected!");
            return nullptr;
            
        case RenderAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(vertices, size); 
        }

        MRG_CORE_ERROR("Unknown graphics API!");
        MRG_CORE_ASSERT(false, "Unknown graphics API!");
        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///                                                                                          ///
    ///                                       Index Buffer                                       ///
    ///                                                                                          ///
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "No graphics API selected!");
            return nullptr;
            
        case RenderAPI::API::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(indices, count); 
        }

        MRG_CORE_ERROR("Unknown graphics API!");
        MRG_CORE_ASSERT(false, "Unknown graphics API!");
        return nullptr;
        
    }
}
