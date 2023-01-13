#pragma once

#include "Mirage/Math/glmTypes.h"
#include "Mirage/ImGui/FontLibrary.h"

#include "ImGui/imgui_internal.h"
#include "ImGui/imgui.h"
#include <functional>

namespace Mirage
{
    static float width = 60.0f;
    static bool DrawSplitUIItem(const char* label, std::function<bool()> fn, const char* Cid = "tab");
    static bool DrawComboBox(const char* label, const char* strings[], int count, const char* preview, int* out);
	
    static bool DrawVec3Control(const char* label, Vec3& vector);

	
	inline ImVec2 operator+(const ImVec2& lhs, const float& rhs)
	{
		return ImVec2(lhs.x + rhs, lhs.y + rhs);
	}
	inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}
	inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
	}
	inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
	}
	inline ImVec2 operator*(const ImVec2& lhs, const float& n)
	{
		return ImVec2(lhs.x * n, lhs.y * n);
	}
	inline ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y);
	}
	inline ImVec2 operator/(const ImVec2& lhs, const float& n)
	{
		return ImVec2(lhs.x / n, lhs.y / n);
	}
}

bool Mirage::DrawSplitUIItem(const char* label, std::function<bool()> fn, const char* Cid)
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const ImVec2 pos = window->DC.CursorPos;
    
    bool result = false;
    if(ImGui::BeginTable(Cid, 2, ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
        ImGui::TableNextRow();
        for (int column = 0; column < 2; column++)
        {
            ImGui::TableSetColumnIndex(column);
            if (column == 0)
            {
                ImVec2 label_pos = ImVec2(pos.x, pos.y + style.FramePadding.y);

                if (label_size.x > 0.0f)
                    RenderText(label_pos, label);
            }
            else
            {
                ImGui::PushItemWidth(-1);
                result = fn();
                ImGui::PopItemWidth();
            }
        }
        ImGui::EndTable();
    }
    return result;
}
bool Mirage::DrawComboBox(const char* label, const char* strings[], int count, const char* preview, int* out)
{
    bool result = false;
    if (ImGui::BeginCombo(label, preview))
    {
        for (int i = 0; i < count; i++)
        {
            bool isSelected = preview == strings[i];

            if (ImGui::Selectable(strings[i], isSelected))
            {
                preview = strings[i];
                *out = i;
                result = true;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
    return result;
}

bool Mirage::DrawVec3Control(const char* label, Vec3& vector)
{
    auto blackFont = GetFont( Font::Black);
    
    bool result = false;

    float lineHeight = blackFont->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 4, lineHeight};
    ImGui::BeginGroup();
    ImGui::PushID(label);
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() - buttonSize.x * 3 + 8);

    static ImVec4 col1 = {0.64f,0.0f,0.0f,1.0f};
    static ImVec4 col2 = {0.0f,0.64f,0.0f,1.0f};
    static ImVec4 col3 = {0.0f,0.0f,0.64f,1.0f};
    
    ImGui::PushID("X");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{-4, 3});
    ImGui::PushStyleColor(ImGuiCol_Button, col1);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col1);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, col1);
    ImGui::PushFont(blackFont);
    ImGui::Button("X", buttonSize);
    ImGui::PopFont();
    ImGui::SameLine();
    result |= ImGui::DragFloat("##X", &vector.x, 0.1f, 0.0f, 0.0f, "%.5g");
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    ImGui::SameLine();
    ImGui::PopItemWidth();
    ImGui::PopID();
    
    ImGui::PushID("Y");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{-4, 3});
    ImGui::PushStyleColor(ImGuiCol_Button, col2);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col2);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, col2);
    ImGui::PushFont(blackFont);
    ImGui::Button("Y", buttonSize);
    ImGui::PopFont();
    ImGui::SameLine();
    result |= ImGui::DragFloat("##Y", &vector.y, 0.1f, 0.0f, 0.0f, "%.5g");
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    ImGui::SameLine();
    ImGui::PopItemWidth();
    ImGui::PopID();
    
    ImGui::PushID("Z");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{-4, 3});
    ImGui::PushStyleColor(ImGuiCol_Button, col3);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col3);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, col3);
    ImGui::PushFont(blackFont);
    ImGui::Button("Z", buttonSize);
    ImGui::PopFont();
    ImGui::SameLine();
    result |= ImGui::DragFloat("##Z", &vector.z, 0.1f, 0.0f, 0.0f, "%.5g");
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    ImGui::SameLine();
    ImGui::PopItemWidth();
    ImGui::PopID();
        
    ImGui::PopID();

    ImGui::EndGroup();

    return result;
}
