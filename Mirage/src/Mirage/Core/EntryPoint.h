#pragma once

#include "Mirage/Core/Core.h"

#ifdef MRG_PLATFORM_WINDOWS

extern Mirage::Application* Mirage::CreateApplication();

int main(int argc, char** argv)
{
    Mirage::Log::Init();

    MRG_PROFILE_BEGIN_SESSION("Initialization", "MirageProfile - Initialization.json");
    
    Mirage::Application* app = Mirage::CreateApplication();

    MRG_PROFILE_END_SESSION();

    MRG_PROFILE_BEGIN_SESSION("Runtime", "MirageProfile - Runtime.json");
    
    app->Run();

    MRG_PROFILE_END_SESSION();

    MRG_PROFILE_BEGIN_SESSION("Termination", "MirageProfile - Termination.json");
    
    delete app;

    MRG_PROFILE_END_SESSION();
}

#endif
