#pragma once

#include "MrgPch.h"

#include "Mirage/Events/Event.h"

namespace Mirage
{
    struct WindowProperties
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        WindowProperties(const std::string& title = "Mirage Engine",
                         uint32_t width = 1200,
                         uint32_t height = 700)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window()
        {
        }

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Scope<Window> Create(const WindowProperties& properties = WindowProperties());
    };
}
