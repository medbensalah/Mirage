#include "ImageButton.h"

namespace Mirage::VisualComponents
{
	ImageButton::ImageButton(ImTextureID& texID, ImVec2 size, ImVec2 uv0, ImVec2 uv1, int framePadding,
		const ImVec4& bgCol, const ImVec4& tintCol)
			: m_TexID(texID), m_Size(size), m_Uv0(uv0), m_Uv1(uv1), m_FramePadding(framePadding), m_BgCol(bgCol), m_TintCol(tintCol)
	{
	}

	void ImageButton::Draw(float scale)
	{
		ImGui::ImageButton(m_TexID, {m_Size.x * scale, m_Size.y * scale}, m_Uv0, m_Uv1, m_FramePadding, m_BgCol, m_TintCol);
	}
}
