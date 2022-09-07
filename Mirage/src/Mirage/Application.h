#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
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

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

    private:
        bool OnWindowClosed(WindowCloseEvent& e);

        LayerStack m_LayerStack;
        
        std::unique_ptr<Window> m_Windows;
        bool m_Running = true;
    };

    /* Define in client */
    Application* CreateApplication();
}
