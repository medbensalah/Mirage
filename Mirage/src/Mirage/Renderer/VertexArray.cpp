﻿#include <MrgPch.h>

#include "Mirage/Renderer/VertexArray.h"
#include "Mirage/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Mirage
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetGfxApi())
        {
        case RenderAPI::API::None:
            MRG_CORE_ASSERT(false, "No graphics API selected!");
            return nullptr;
            
        case RenderAPI::API::OpenGL:
            return CreateRef<OpenGLVertexArray>(); 
        }

        MRG_CORE_ERROR("Unknown graphics API!");
        MRG_CORE_ASSERT(false, "Unknown graphics API!");
        return nullptr;
    }
}
