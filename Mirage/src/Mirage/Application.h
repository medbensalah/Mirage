#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/ApplicationEvent.h"

#include "Mirage/ImGui/ImGuiLayer.h"

#include "Mirage/Renderer/Shader.h"
#include "Mirage/Renderer/Buffer.h"

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

        unsigned int m_VertexArray;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;

        static Application* s_Instance;
    };

    /* Define in client */
    Application* CreateApplication();
}
