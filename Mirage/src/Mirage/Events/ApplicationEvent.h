#pragma once

#include "Event.h"
#include "Mirage/Core/Math.h"

namespace Mirage
{
    MIRAGE_CLASS WindowResizeEvent : public Event
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

    MIRAGE_CLASS WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };

    MIRAGE_CLASS AppTickEvent : public Event
    {
    public:
        AppTickEvent() {}

        EVENT_CLASS_TYPE(AppTick);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };

    MIRAGE_CLASS AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() {}

        EVENT_CLASS_TYPE(AppUpdate);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };
    
    MIRAGE_CLASS AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() {}

        EVENT_CLASS_TYPE(AppRender);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    };
}
