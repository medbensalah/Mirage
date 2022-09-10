#include "MrgPch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

#include <glad/glad.h>

namespace Mirage
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        MRG_CORE_ASSERT(windowHandle, "Null refrence to WindowHandle");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        MRG_CORE_ASSERT(status, "Glad initialization Failed!");

        MRG_CORE_INFO("OpenGL Info:");
        MRG_CORE_INFO("\tVendor: {0}", glGetString(GL_VENDOR));
        MRG_CORE_INFO("\tOpenGL renderer : {0}", glGetString(GL_RENDERER));
        MRG_CORE_INFO("\tOpenGL version : {0}", glGetString(GL_VERSION));
        
    }

    void OpenGLContext::SwapBuffers()
    { 
        glfwSwapBuffers(m_WindowHandle);
    }
}
