#pragma once

#include "Core.h"
#include "Window.h"

namespace Mirage
{
    MIRAGE_CLASS Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        std::unique_ptr<Window> m_Windows;

        bool m_Running = true;
    };

    /* Define in client */
    Application* CreateApplication();
}
