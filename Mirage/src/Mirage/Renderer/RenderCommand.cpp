#include <MrgPch.h>

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"


namespace Mirage
{
    Scope<RenderAPI> RenderCommand::s_RenderAPI = CreateScope<OpenGLRenderAPI>();
}
