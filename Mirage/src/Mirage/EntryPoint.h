#pragma once

#ifdef MRG_PLATFORM_WINDOWS

extern Mirage::Application* Mirage::CreateApplication();

int main(int argc, char** argv)
{
    Mirage::Log::Init();

    Mirage::Application* app = Mirage::CreateApplication();
    app->Run();
    delete app;
}

#endif
