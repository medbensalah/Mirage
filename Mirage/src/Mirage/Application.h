#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace Mirage
{
    MIRAGE_CLASS Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);
        
    private:
bool OnWindowClosed(WindowCloseEvent& e);
        
        std::unique_ptr<Window> m_Windows;

        bool m_Running = true;
    };

    /* Define in client */
    Application* CreateApplication();
}
