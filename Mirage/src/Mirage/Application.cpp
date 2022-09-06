#include "MrgPch.h"

#include "Application.h"

#include "Mirage/Events/ApplicationEvent.h"
#include "Mirage/Log.h"

namespace Mirage
{
    Application::Application()
    {
        m_Windows = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_Windows->OnUpdate();
        }
    }
}
