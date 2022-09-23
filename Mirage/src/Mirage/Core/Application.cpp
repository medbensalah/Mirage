﻿#include "MrgPch.h"

#include "Mirage/Core/Application.h"

#include "Mirage/Core/Time.h"
#include "GLFW/glfw3.h"
#include "Mirage/Renderer/Renderer.h"

namespace Mirage
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        MRG_PROFILE_FUNCTION();
        
        MRG_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Window::Create();
        m_Window->SetVSync(false);
        m_Window->SetEventCallback(MRG_BIND_EVENT_FN(Application::OnEvent));

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
    
    void Application::OnEvent(Event& e)
    {
        MRG_PROFILE_FUNCTION();
        
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(MRG_BIND_EVENT_FN(Application::OnWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(MRG_BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        MRG_PROFILE_FUNCTION();
        
        while (m_Running)
        {
            MRG_PROFILE_SCOPE("RUN LOOP");
            time.Update((float)glfwGetTime());

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

    bool Application::OnWindowClosed(WindowCloseEvent& e)
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
