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

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;
        
        void Begin();
        void End();
        
    private:
        float m_Time = 0.0f;
    };
}
