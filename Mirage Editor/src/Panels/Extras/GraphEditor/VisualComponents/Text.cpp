#include "Text.h"

namespace Mirage::VisualComponents
{
	Text::Text(std::string text, ImVec4 color, float horizontalAlign, float verticalAlign, int fontSize)
		: m_Text(text), m_FontSize(fontSize), m_Color(color), m_HorizontalAlign(horizontalAlign), m_VerticalAlign(verticalAlign)
	{
	}

	void Text::Draw(float scale)
	{	
		ImGui::PushStyleColor(ImGuiCol_Text, m_Color);
		ImFont* font = ImGui::GetIO().FontDefault;
		font->Scale = m_FontSize / font->FontSize;
		ImGui::PushFont(font);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * m_HorizontalAlign);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * m_VerticalAlign);
		ImGui::Text(m_Text.c_str());
		ImGui::PopFont();
		ImGui::PopStyleColor();
	}
}
