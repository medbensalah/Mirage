#pragma once

#include "Event.h"

//TODO change mouse coordinates to vect2

namespace Mirage
{
    MIRAGE_CLASS MouseMoveEvent : public Event
    {
    public:
        MouseMoveEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y)
        {
        }

        inline float GetX() const { return m_MouseX; }
        inline float GetY() const { return m_MouseY; }

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

    MIRAGE_CLASS MouseScrollEvent : public Event
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

    
    MIRAGE_CLASS MouseButtonEvent : public Event
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
    MIRAGE_CLASS MouseButtonPressed : public MouseButtonEvent
    {
    public:
        MouseButtonPressed(int button)
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
    MIRAGE_CLASS MouseButtonReleased : public MouseButtonEvent
    {
    public:
        MouseButtonReleased(int button)
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
