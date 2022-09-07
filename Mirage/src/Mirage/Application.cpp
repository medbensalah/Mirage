#include "MrgPch.h"

#include "Application.h"

#include <GLFW/glfw3.h>

namespace Mirage
{
#define Bind_event_FN(x) std::bind(x, this, std::placeholders::_1)
    
    Application::Application()
    {
        m_Windows = std::unique_ptr<Window>(Window::Create());
        m_Windows->SetEventCallbackFn(Bind_event_FN(&Application::OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }
    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }
    

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(Bind_event_FN(&Application::OnWindowClosed));
        MRG_CORE_TRACE(e);

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
            glClearColor(0.2f, 0.32f, 0.83f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            
            m_Windows->OnUpdate();
        }
    }
    
    bool Application::OnWindowClosed(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }

}
