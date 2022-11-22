#pragma once

#include "ImGui/imgui.h"

namespace ImGui
{
	static void DrawSplitter(int split_vertically, float thickness, float* size0, float* size1, float min_size0,
	                  float min_size1);
}

void ImGui::DrawSplitter(int split_vertically, float thickness, float* size0, float* size1, float min_size0, float min_size1)
{
	ImVec2 backup_pos = ImGui::GetCursorPos();
	if (split_vertically)
		ImGui::SetCursorPosY(backup_pos.y + *size0);
	else
		ImGui::SetCursorPosX(backup_pos.x + *size0);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
	// We don't draw while active/pressed because as we move the panes the splitter button will be 1 frame late
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.10f));
	ImGui::Button("##Splitter", ImVec2(!split_vertically ? thickness : -1.0f, split_vertically ? thickness : -1.0f));
	ImGui::PopStyleColor(3);

	ImGui::SetItemAllowOverlap(); // This is to allow having other buttons OVER our splitter. 

	if (ImGui::IsItemHovered())
	{
		ImGui::SetMouseCursor(split_vertically ? ImGuiMouseCursor_ResizeNS : ImGuiMouseCursor_ResizeEW);
	}
	if (ImGui::IsItemActive())
	{
		ImGui::SetMouseCursor(split_vertically ? ImGuiMouseCursor_ResizeNS : ImGuiMouseCursor_ResizeEW);
		float mouse_delta = split_vertically ? ImGui::GetIO().MouseDelta.y : ImGui::GetIO().MouseDelta.x;

		// Minimum pane size
		if (mouse_delta < min_size0 - *size0)
			mouse_delta = min_size0 - *size0;
		if (mouse_delta > *size1 - min_size1)
			mouse_delta = *size1 - min_size1;

		// Apply resize
		*size0 += mouse_delta;
		*size1 -= mouse_delta;
	}
	ImGui::SetCursorPos(backup_pos);
}
