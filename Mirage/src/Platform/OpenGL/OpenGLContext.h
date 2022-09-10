#pragma once
#include "Mirage/Renderer/Graphicscontext.h"

struct GLFWwindow;

namespace Mirage 
{
    class OpenGLContext: public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        
        virtual void Init() override;
        virtual void SwapBuffers() override;
    
    
    private:
        GLFWwindow* m_WindowHandle;
    };
}
