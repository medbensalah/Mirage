#pragma once

#include "Mirage/Layer.h"

#include "Mirage/Events/ApplicationEvent.h"
#include "Mirage/Events/KeyEvent.h"
#include "Mirage/Events/MouseEvent.h"

namespace Mirage
{
    MIRAGE_CLASS ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& event) override;
        
    private:
        bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
        bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
        bool OnMouseMoved(MouseMoveEvent& event);
        bool OnMouseScroll(MouseScrollEvent& event);
        
        bool OnKeyPressed(KeyPressedEvent& event);
        bool OnKeyReleased(KeyReleasedEvent& event);
        bool OnKeyTyped(KeyTypedEvent& event);

        bool OnWindowResize(WindowResizeEvent& event);
            
        
        float m_Time = 0.0f;
    };
}
