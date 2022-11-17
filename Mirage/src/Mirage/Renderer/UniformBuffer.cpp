#include "MrgPch.h"
#include "UniformBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Mirage
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetGfxApi())
		{
		case RenderAPI::API::None:    MRG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		MRG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
