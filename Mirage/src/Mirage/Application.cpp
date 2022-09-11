#include "MrgPch.h"

#include "Application.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace Mirage
{
#define Bind_event_FN(x) std::bind(x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        MRG_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallbackFn(Bind_event_FN(&Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        float vertices[3 * 7] = {
            -1.0f, -1.0f, 0.f,
             1.0f, -1.0f, 0.f,
             1.0f,  1.0f, 0.f,
            -1.0f,  1.0f, 0.f,
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

        uint32_t indices[] = {
            0, 1, 2,
            0, 2, 3
        };
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        std::string vertexShader = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;

        out vec3 v_Position;
        
        void main()
        {
            v_Position = a_Position;
            gl_Position = vec4(a_Position, 1.0);
        }
        )";

        std::string fragmentShader = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Position;
        
        void main()
        {
            color = vec4(v_Position + vec3(1.0,1.0,0.5), 1.0);
        }
        )";

        m_Shader.reset(new Shader(vertexShader, fragmentShader));
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }


    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(Bind_event_FN(&Application::OnWindowClosed));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
            glDrawArrays(GL_TRIANGLES, 0, 3);


            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }


            m_ImGuiLayer->Begin();

            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }

            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClosed(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }
}
