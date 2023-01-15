#pragma once
#include <string>

#include "Mirage/Core/Log.h"
#include "VisualComponent.h"
#include "ImGui/imgui.h"

namespace Mirage::VisualComponents
{
	class Text : public VisualComponent
	{
	public:
		Text(std::string text, ImVec4 color = ImVec4(255,255,255,255), float horizontalAlign = 0.5f, float verticalAlign = 0.5f, int fontSize = 18);
		~Text()
		{
		}
		void Draw(float scale) override;

		// Getters and Setters
		std::string GetText() const { return m_Text; }
		void SetText(std::string text) { m_Text = text; }

		int GetFontSize() const { return m_FontSize; }
		void SetFontSize(int fontSize) { m_FontSize = fontSize; }

		ImVec4 GetColor() const { return m_Color; }
		void SetColor(ImVec4 color) { m_Color = color; }

		float GetHorizontalAlign() const { return m_HorizontalAlign; }
		void SetHorizontalAlign(float horizontalAlign) { m_HorizontalAlign = horizontalAlign; }

		float GetVerticalAlign() const { return m_VerticalAlign; }
		void SetVerticalAlign(float verticalAlign) { m_VerticalAlign = verticalAlign; }

	private:
		std::string m_Text;
		int m_FontSize;
		ImVec4 m_Color;
		float m_HorizontalAlign;
		float m_VerticalAlign;
	};
}
