#pragma once
#include "VisualComponent.h"
#include "Mirage/Core/Core.h"

namespace Mirage::VisualComponents
{
	class Port;

	class Link : public VisualComponent
	{
	public:
		Link() {}
		~Link() {}
		void Draw(float scale) override;
		bool IsHovered(float scale);

		void SetPort(Port* port);
		void SetColor(const ImColor& color);
		Port* m_InputPort = nullptr;
		Port* m_OutputPort = nullptr;

	private:
		void ForwardData();
		
	private:
		ImVec2 m_Start = { 0.0f, 0.0f };
		ImVec2 m_End = { 0.0f, 0.0f };

		ImVec2 m_StartTangent = { 0.0f, 0.0f };
		ImVec2 m_EndTangent = { 0.0f, 0.0f };

		ImColor m_Color = ImColor(255, 255, 255, 255);
		ImColor m_HoveredColor = ImColor{75, 181, 242, 255};
		ImColor m_GrayedColor = ImColor{255, 255, 255, 128};

		float m_Thickness = 3.0f;
		float m_HoveredThickness = 5.0f;
		float m_GrayedThickness = 5.0f;
		
		float m_TgLength = 125.0f;
	};
}
