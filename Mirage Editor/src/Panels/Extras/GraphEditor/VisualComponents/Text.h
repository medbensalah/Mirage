#pragma once

#include <string>

#include "Mirage/ImGui/FontLibrary.h"

namespace Mirage::VisualComponents
{
	class Text : public VisualComponent
	{
	public:
		Text(const std::string& text, ImVec4 color = ImVec4(255, 255, 255, 255), int fontSize = 18, bool bold = false);

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

	private:
		std::string m_Text;
		int m_FontSize;
		ImVec4 m_Color;
		bool m_Bold;
	};
}
