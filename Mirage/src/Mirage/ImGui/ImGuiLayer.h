    #pragma once

#include "Mirage/Core/Layer.h"

namespace Mirage
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        
		virtual void OnEvent(Event& e) override;
        
        void Begin();
        void End();

        void AllowKbEvents(bool allow) { m_BlockKbEvents = !allow; }
        void AllowMouseEvents(bool allow) { m_BlockMouseEvents = !allow; }
        
    private:
        bool m_BlockKbEvents = true;
        bool m_BlockMouseEvents = true;
        float m_Time = 0.0f;
    };
}
