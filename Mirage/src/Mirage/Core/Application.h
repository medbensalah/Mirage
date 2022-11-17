#pragma once

#include "Mirage/Core/Core.h"

#include "Mirage/Core/Window.h"
#include "Mirage/Core/LayerStack.h"
#include "Mirage/Core/Time.h"
#include "Mirage/Events/ApplicationEvent.h"

#include "Mirage/ImGui/ImGuiLayer.h"
int main(int argc, char** argv);

namespace Mirage
{
    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            MRG_CORE_ASSERT(index < Count);
            return Args[index];
        }
    };
    
    class Application
    {
    public:
        Application(const std::string& name = "Mirage", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
        virtual ~Application();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void Close();
        
        inline Window& GetWindow() { return *m_Window; }

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        inline static Application& Get() { return *s_Instance; }
    
        inline static float GetSeconds() { return s_Instance->time.GetElapsedTimeInSeconds(); }
        inline static float GetMilliseconds() { return s_Instance->time.GetElapsedTimeInMilliseconds(); }

        inline static float GetDeltaTime() { return s_Instance->time.DeltaTime; }
        
        ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
    private:
        void Run();

        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        ApplicationCommandLineArgs m_CommandLineArgs;
        
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
    Application* CreateApplication(ApplicationCommandLineArgs args);
}
