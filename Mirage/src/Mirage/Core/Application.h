﻿#pragma once

#include "Mirage/Core/Core.h"

#include "Mirage/Core/Window.h"
#include "Mirage/Core/LayerStack.h"
#include "Mirage/Core/Time.h"
#include "Mirage/Events/ApplicationEvent.h"

#include "Mirage/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Mirage
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }

        inline static Application& Get() { return *s_Instance; }
    
        inline static float GetSeconds() { return s_Instance->time.GetElapsedTimeInSeconds(); }
        inline static float GetMilliseconds() { return s_Instance->time.GetElapsedTimeInMilliseconds(); }

        inline static float GetDeltaTime() { return s_Instance->time.DeltaTime; }
    private:
        void Run();

        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        LayerStack m_LayerStack;

        Time time = Time(0.0f);

        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;

        bool m_Running = true;
        bool m_Minimized = false;

        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
    };

    /* Define in client */
    Application* CreateApplication();
}
