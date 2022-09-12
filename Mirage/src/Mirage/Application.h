#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Core/Time.h"
#include "Events/ApplicationEvent.h"

#include "Mirage/ImGui/ImGuiLayer.h"

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

        inline Window& GetWindow() { return *m_Window; }

        inline static Application& Get() { return *s_Instance; }
    
        inline static float GetSeconds() { return s_Instance->time.GetSeconds(); }
        inline static float GetMilliseconds() { return s_Instance->time.GetMilliseconds(); }
    private:
        bool OnWindowClosed(WindowCloseEvent& e);

        LayerStack m_LayerStack;

        Time time = Time(0.0f);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;

        bool m_Running = true;

        static Application* s_Instance;
    };

    /* Define in client */
    Application* CreateApplication();
}
