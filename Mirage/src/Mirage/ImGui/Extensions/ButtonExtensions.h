#pragma once

#include "ImGui/imgui.h"

// Header
namespace ImGui
{
    
    static bool GradientButton(const char* label, const ImVec2& size = ImVec2(-FLT_MIN, 30));
    
    static bool GradientImageButton(ImTextureID user_texture_id, const ImVec2& size = ImVec2(-FLT_MIN, 30));

    static bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f, const ImVec2& psize = ImVec2(-FLT_MIN, 30));
}

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui_internal.h"

// Implementation
bool ImGui::GradientButton(const char* label, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    ImVec2 maxSize = {pos.x + size.x, pos.y + size.y};
    const ImRect bb(pos, maxSize);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    ImGuiButtonFlags flags = ImGuiButtonFlags_None;
    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    
    ImVec4* colors = ImGui::GetStyle().Colors;

    ImU32 bg_color_1 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonU]);
    ImU32 bg_color_2 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonD]);
    
    if(hovered)
    {
        bg_color_1 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonHoveredU]);
        bg_color_2 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonHoveredD]);
    }
    if(held)
    {
        bg_color_1 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonActiveU]);
        bg_color_2 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonActiveD]);
    }
    RenderNavHighlight(bb, id);

    int vert_start_idx = window->DrawList->VtxBuffer.Size;
    window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_color_1, g.Style.FrameRounding);
    int vert_end_idx = window->DrawList->VtxBuffer.Size;
    // if (is_gradient)
        ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vert_start_idx, vert_end_idx, bb.Min, bb.GetBL(), bg_color_1, bg_color_2);
    if (g.Style.FrameBorderSize > 0.0f) 
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), g.Style.FrameRounding, 0, g.Style.FrameBorderSize);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");
    
    ImVec2 posMin = {bb.Min.x - style.FramePadding.x , bb.Min.y  + style.FramePadding.y - label_size.y / 2};
    
    ImVec2 posMax = {bb.Max.x + style.FramePadding.x, bb.Max.y + style.FramePadding.y};
    RenderTextClipped(posMin,posMax, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImGui::GradientImageButton(ImTextureID user_texture_id, const ImVec2& size_arg)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID("#image");
	const ImVec2 label_size = size_arg;

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	ImVec2 maxSize = {pos.x + size.x, pos.y + size.y};
	const ImRect bb(pos, maxSize);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	ImGuiButtonFlags flags = ImGuiButtonFlags_None;
	if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	
	ImVec4* colors = ImGui::GetStyle().Colors;

	ImU32 bg_color_1 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonU]);
	ImU32 bg_color_2 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonD]);
	
	if(hovered)
	{
		bg_color_1 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonHoveredU]);
		bg_color_2 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonHoveredD]);
	}
	if(held)
	{
		bg_color_1 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonActiveU]);
		bg_color_2 = ImGui::GetColorU32(colors[ImGuiCol_GradButtonActiveD]);
	}
	RenderNavHighlight(bb, id);

	int vert_start_idx = window->DrawList->VtxBuffer.Size;
	window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_color_1, g.Style.FrameRounding);
	int vert_end_idx = window->DrawList->VtxBuffer.Size;
	// if (is_gradient)
		ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vert_start_idx, vert_end_idx, bb.Min, bb.GetBL(), bg_color_1, bg_color_2);
	if (g.Style.FrameBorderSize > 0.0f)
		window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), g.Style.FrameRounding, 0, g.Style.FrameBorderSize);

	if (g.LogEnabled)
		LogSetNextTextDecoration("[", "]");

	ImVec2 posMin = {bb.Min.x - style.FramePadding.x , bb.Min.y  + style.FramePadding.y - label_size.y / 2};
	
	ImVec2 posMax = {bb.Max.x + style.FramePadding.x, bb.Max.y + style.FramePadding.y};
	window->DrawList->AddImage(user_texture_id, posMin, posMax);

	return pressed;
}


bool ImGui::ButtonCenteredOnLine(const char* label, float alignment, const ImVec2& psize)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImMax(ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f, psize.x) ;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return GradientButton(label, ImVec2(size, psize.y));
}
