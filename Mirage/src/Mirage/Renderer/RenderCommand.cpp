#include <MrgPch.h>

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"


namespace Mirage
{
    RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI();
}
