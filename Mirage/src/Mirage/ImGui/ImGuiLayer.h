#pragma once

#include "Mirage/Core/Layer.h"

namespace Mirage
{
    class ImGuiLayer : public Layer
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
