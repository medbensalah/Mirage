#pragma once
#include "Core.h"

namespace Mirage
{
    class MIRAGE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    /* Define in client */
    Application* CreateApplication();
}
