#include "MrgPch.h"

#include "Platform/Windows/WindowsWindow.h"

#include "Mirage/Events/ApplicationEvent.h"
#include "Mirage/Events/KeyEvent.h"
#include "Mirage/Events/MouseEvent.h"

#include "Mirage/Core/KeyCodes.h"
#include "Mirage/Core/MouseCodes.h"

#include "Mirage/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Mirage
{
    //static bool s_GLFWInitialized = false;
    static uint8_t s_GLFWWindowCount = 0;
    
    static void GLFWErrorCallback(int error, const char* description)
    {
        MRG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const WindowProperties& properties)
    {
        MRG_PROFILE_FUNCTION();
        Init(properties);
    }

    WindowsWindow::~WindowsWindow()
    {
        MRG_PROFILE_FUNCTION();

        Shutdown();
    }

    void WindowsWindow::Init(const WindowProperties& properties)
    {
        MRG_PROFILE_FUNCTION();
        
        m_Data.Title = properties.Title;
        m_Data.Width = properties.Width;
        m_Data.Height = properties.Height;
        

        MRG_CORE_INFO("Creating window \"{0}\" with size ({1}, {2})",
                      properties.Title,
                      properties.Width,
                      properties.Height
        );

        if (s_GLFWWindowCount == 0)
        {
            MRG_PROFILE_SCOPE("glfwInit");
            
            int success = glfwInit();
            MRG_CORE_ASSERT(success, "GLFW initialization Failed!");
            glfwSetErrorCallback(GLFWErrorCallback);
            //s_GLFWInitialized = true;
        }

        {
            MRG_PROFILE_SCOPE("glfwCreateWindow");

#if defined(MRG_DEBUG)
            if (Renderer::GetGfxApi() == RenderAPI::API::OpenGL)
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
            
            m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
            ++s_GLFWWindowCount;
        }
        m_Context = GraphicsContext::Create(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        /* Set GLFW callbacks */
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scanmode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
            case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(character);

            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
            case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrollEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMoveEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });

    }

    void WindowsWindow::Shutdown()
    {
        MRG_PROFILE_FUNCTION();

        glfwDestroyWindow(m_Window);
        --s_GLFWWindowCount;
        
        if (s_GLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }

    void WindowsWindow::OnUpdate()
    {
        MRG_PROFILE_FUNCTION();

        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        MRG_PROFILE_FUNCTION();

        if (enabled)
        {
            MRG_CORE_TRACE("Enabling VSync");
            glfwSwapInterval(1);
        }
        else
        {
            MRG_CORE_TRACE("Disabling VSync");
            glfwSwapInterval(0);
        }

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
