#include <MrgPch.h>

#include "Mirage/Renderer/RenderCommand.h"


namespace Mirage
{
    Scope<RenderAPI> RenderCommand::s_RenderAPI = RenderAPI::Create();
}
