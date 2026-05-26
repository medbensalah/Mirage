#pragma once

#include "Mirage/Events/Event.h"
#include "Mirage/Math/glmTypes.h"
#include <vector>

namespace Mirage
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_Width(width), m_Height(height)
        {
        }

        inline Vec2 GetWindowBounds() { return {m_Width, m_Height}; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Event: (" << GetName() << ") Size: " << "(" << m_Width << ", " << m_Height << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);

    private:
        unsigned int m_Width, m_Height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };

    class WindowFileDropEvent : public Event
    {
    public:
        WindowFileDropEvent(std::vector<std::string> paths)
            : m_Paths(std::move(paths))
        {
        }

        const std::vector<std::string>& GetPaths() const { return m_Paths; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Event: (" << GetName() << ") Count: " << m_Paths.size();
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowFileDrop);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    private:
        std::vector<std::string> m_Paths;
    };

    class AppTickEvent : public Event
    {
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };

    class AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };
    
    class AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };
}
