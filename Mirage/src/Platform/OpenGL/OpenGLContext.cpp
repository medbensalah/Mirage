#include "MrgPch.h"
#include "Platform/OpenGL/OpenGLContext.h"

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
        MRG_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        MRG_CORE_ASSERT(status, "Glad initialization Failed!");

        MRG_CORE_INFO("OpenGL Info:");
        MRG_CORE_INFO("\tVendor: {0}", glGetString(GL_VENDOR));
        MRG_CORE_INFO("\tOpenGL renderer : {0}", glGetString(GL_RENDERER));
        MRG_CORE_INFO("\tOpenGL version : {0}", glGetString(GL_VERSION));


        MRG_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Mirage requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        MRG_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}
