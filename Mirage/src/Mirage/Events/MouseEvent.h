#pragma once

#include "Mirage/Events/Event.h"

namespace Mirage
{
    class MouseMoveEvent : public Event
    {
    public:
        MouseMoveEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y)
        {
        }

        inline Vec2 GetMousePosition() { return {m_MouseX, m_MouseY }; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Event: (" << GetName() << ") Position: " << "(" << m_MouseX << ", " << m_MouseY << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMove);
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

    private:
        float m_MouseX, m_MouseY;
    };

    class MouseScrollEvent : public Event
    {
    public:
        MouseScrollEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset)
        {
        }

        inline float GetXOffset() const { return m_XOffset; }
        inline float GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Event: (" << GetName() << ") Offset: " << "(" << m_XOffset << ", " << m_YOffset << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScroll);
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

    private:
        float m_XOffset, m_YOffset;
    };

    
    class MouseButtonEvent : public Event
    {
    public:
        int GetButton() const { return m_Button; }
        
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

    protected:
        MouseButtonEvent(int button)
            : m_Button(button)
        {
        }

        int m_Button;
    };
    
    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Event: (" << GetName() << ") Button: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed);
    };
    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Event: (" << GetName() << ") Button: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased);
    };
}
