#pragma once
#include "ImGui/imgui.h"

namespace Mirage::Graph::Style
{
	static float GridSpacing = 50.0f;
	static ImU32 BackgroundColor = ImColor(39, 39, 39, 255);
	static ImU32 ThinLineColor = ImColor(120, 120, 120, 64);
	static float ThinLineWidth = 1.0f;

	static ImU32 NodeColor = IM_COL32(17, 17, 17, 200);
	static ImU32 NodeBorderColor = IM_COL32(255, 255, 255, 96);
	static ImU32 NodeBorderColorHovered = IM_COL32(0, 183, 255, 128);
	static ImU32 NodeBorderColorSelected = IM_COL32(255, 183, 0, 128);
};

