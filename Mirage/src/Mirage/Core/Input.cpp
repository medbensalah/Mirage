#include "MrgPch.h"
#include "Mirage/Core/Input.h"

#ifdef MRG_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace Mirage
{
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
#ifdef MRG_PLATFORM_WINDOWS
        return CreateScope<WindowsInput>();
#else
        MRG_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
}
