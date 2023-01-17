#include "Button.h"

namespace Mirage::VisualComponents
{
	Button::Button(const std::string& label, ImVec2& size)
		: m_Label(label), m_Size(size)
	{
	}

	void Button::Draw(float scale)
	{
		ImGui::Button(m_Label.c_str(), ImVec2(m_Size.x * scale, m_Size.y * scale));
	}
}
