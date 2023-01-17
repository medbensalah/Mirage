#include "VisualComponent.h"

#include "Text.h"

namespace Mirage::VisualComponents
{
	Text::Text(const std::string& text, ImVec4 color, int fontSize, bool bold)
		: m_Text(text), m_FontSize(fontSize), m_Color(color), m_Bold(bold)
	{
	}

	void Text::Draw(float scale)
	{			
		ImFont* font = GetFont(m_Bold? Font::Black : Font::Regular);
		font->Scale = m_FontSize / font->FontSize;
		ImGui::PushFont(font);
		ImGui::TextColored(m_Color, m_Text.c_str());
		font->Scale = 1.0f;
		ImGui::PopFont();
	}
}
