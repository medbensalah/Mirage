#include "Text.h"

#include "Mirage/ImGui/FontLibrary.h"

namespace Mirage::VisualComponents
{
	Text::Text(std::string text, ImVec4 color, int fontSize, bool bold)
		: m_Text(text), m_FontSize(fontSize), m_Color(color), m_Bold(bold)
	{
	}

	void Text::Draw(float scale)
	{	
		ImGui::PushStyleColor(ImGuiCol_Text, m_Color);
		
		ImFont* font = GetFont(m_Bold? Font::Black : Font::Regular);
		// ImFont* font = ImGui::GetIO().Fonts->Fonts[0];
		font->Scale = m_FontSize / font->FontSize;
		ImGui::PushFont(font);
		ImGui::Text(m_Text.c_str());
		font->Scale = 1.0f;
		ImGui::PopFont();
		ImGui::PopStyleColor();
	}
}
