#pragma once

#include "Mirage/Window.h"
#include "Mirage/Renderer/Graphicscontext.h"

#include <GLFW/glfw3.h>

namespace Mirage
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProperties& properties);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        inline void SetEventCallbackFn(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
        
        inline virtual void* GetNativeWindow() const override { return m_Window; }

    private:
        virtual void Init(const WindowProperties& properties);
        virtual void Shutdown();

        GLFWwindow* m_Window;
        GraphicsContext* m_Context;
        
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}
