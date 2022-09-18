#pragma once

#include "Core.h"
#include "Mirage/Events/Event.h"

namespace Mirage
{
    MIRAGE_CLASS Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }

        private:
        std::string m_DebugName;
    };
}
