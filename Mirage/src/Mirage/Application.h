#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/ApplicationEvent.h"

#include "Mirage/ImGui/ImGuiLayer.h"

#include "Mirage/Renderer/Shader.h"
#include "Renderer/VertexArray.h"

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

    private:
        bool OnWindowClosed(WindowCloseEvent& e);

        LayerStack m_LayerStack;

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;
        
        std::shared_ptr<Shader> m_SquareShader;
        std::shared_ptr<VertexArray> m_SquareVA;

        static Application* s_Instance;
    };

    /* Define in client */
    Application* CreateApplication();
}
