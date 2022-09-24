#include "MrgPch.h"
#include "Mirage/Core/Window.h"

#ifdef MRG_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Mirage
{

    Scope<Window> Window::Create(const WindowProperties& properties)
    {
#ifdef MRG_PLATFORM_WINDOWS
        return CreateScope<WindowsWindow>(properties);
#else
        MRG_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }

}