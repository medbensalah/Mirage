#include "MrgPch.h"

#include "Mirage/Core/Application.h"

#include "Mirage/Core/Time.h"
#include "GLFW/glfw3.h"
#include "Mirage/Renderer/Renderer.h"
#include "Mirage/utils/PlatformUtils.h"

namespace Mirage
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name, ApplicationCommandLineArgs args)
        : m_CommandLineArgs(args)
    {
        MRG_PROFILE_FUNCTION();
        
        MRG_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = Window::Create(WindowProperties(name));
        m_Window->SetEventCallback(MRG_BIND_EVENT_FN(Application::OnEvent));

        m_Window->SetVSync(false);
        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }
    
    Application::~Application()
    {
        MRG_PROFILE_FUNCTION();
        
        Renderer::Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        MRG_PROFILE_FUNCTION();
        
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        MRG_PROFILE_FUNCTION();
        
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::OnEvent(Event& e)
    {
        MRG_PROFILE_FUNCTION();
        
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(MRG_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(MRG_BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (e.Handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    void Application::Run()
    {
        MRG_PROFILE_FUNCTION();
        
        while (m_Running)
        {
            MRG_PROFILE_SCOPE("RUN LOOP");
            time.Update(TimeUtils::GetTime());
        	
            if(!m_Minimized)
            {
                {
                    MRG_PROFILE_SCOPE("Layerstack Update");
                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(time.DeltaTime);
                }
                
                m_ImGuiLayer->Begin();
                {
                    MRG_PROFILE_SCOPE("layerstack ImGui Render");
                    for (Layer* layer : m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }


            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return false;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        MRG_PROFILE_FUNCTION();
        
        if (e.GetWindowBounds().x == 0 || e.GetWindowBounds().y == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;

        Renderer::OnWindowResize(e.GetWindowBounds());
        
        return false;
    }
}
