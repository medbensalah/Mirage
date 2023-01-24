#pragma once

#include "ImGui/imgui.h"
#include <vector>

namespace Mirage::VisualComponents
{
	class VisualComponent
	{
	public:
		VisualComponent() {}
		VisualComponent(const VisualComponent& vc) = default;
		
		virtual ~VisualComponent() {}

		virtual void Draw(float scale) {}
	};

	class SameLineSpacing : public VisualComponent
	{
	public:
		SameLineSpacing(float spacingX) : m_SpacingX(spacingX) {}
		virtual ~SameLineSpacing() override {}
		
		virtual void Draw(float scale)
		{
			ImGui::SameLine(0, m_SpacingX * scale);
		}
	private:
		float m_SpacingX;
	};
	class LineSpacing : public VisualComponent
	{
	public:
		LineSpacing(float spacingY) : m_SpacingY(spacingY) {}
		virtual ~LineSpacing() override {}
		
		virtual void Draw(float scale)
		{
			ImGui::Spacing();
			ImGui::Dummy(ImVec2(0, m_SpacingY * scale));
		}
	private:
		float m_SpacingY;
	};
}
