#include "Button.h"

#include "ImGui/imgui.h"

namespace Mirage::VisualComponents
{
	void Button::Draw(float scale)
	{
		ImGui::Button(m_Label.c_str());
	}
}
