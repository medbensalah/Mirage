#pragma once

#include <string>

#include "VisualComponent.h"

namespace Mirage::VisualComponents
{
	class ImageButton : public VisualComponent
	{
	public:
		ImageButton(ImTextureID& texID, ImVec2 size, ImVec2 uv0 = {0, 0}, ImVec2 uv1 = {1, 1}, int framePadding = -1, const ImVec4& bgCol = {0, 0, 0, 0}, const ImVec4& tintCol = {1, 1, 1, 1});

		virtual ~ImageButton() override {}

		virtual void Draw(float scale) override;

	private:
		ImTextureID& m_TexID;
		ImVec2 m_Size;
		ImVec2 m_Uv0;
		ImVec2 m_Uv1;
		int m_FramePadding;
		ImVec4 m_BgCol;
		ImVec4 m_TintCol;
	};
}
