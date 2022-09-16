#include "MrgPch.h"

#include "Application.h"

#include "Core/Time.h"
#include "GLFW/glfw3.h"
#include "Mirage/Renderer/Renderer.h"

namespace Mirage
{
#define Bind_event_FN(x) std::bind(x, this, std::placeholders::_1)
    
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        MRG_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallbackFn(Bind_event_FN(&Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
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
            time.Update((float)glfwGetTime());
            
            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate(time.DeltaTime);
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
