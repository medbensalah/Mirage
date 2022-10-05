#pragma once

namespace ImGui
{
    bool ToggleButton(const char* label, bool* v)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;
        
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        
        ImVec4* colors = ImGui::GetStyle().Colors;
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        float height = ImGui::GetFrameHeight();
        float width = height * 1.55f;
        float radius = height * 0.50f;

        ImGui::InvisibleButton(label, ImVec2(width, height));
        if (ImGui::IsItemClicked()) *v = !*v;
        ImGuiContext& gg = *GImGui;
        
        
        const ImGuiStyle& style = gg.Style;
        float ANIM_SPEED = 0.085f;
    const ImRect check_bb(p, ImVec2(p.x + width, p.y + height));
    ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
        if (label_size.x > 0.0f)
            RenderText(label_pos, label);
        if (gg.LastActiveId == gg.CurrentWindow->GetID(label))// && g.LastActiveIdTimer < ANIM_SPEED)
            float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
        if (ImGui::IsItemHovered())
            draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(colors[ImGuiCol_ButtonHovered]), height * 0.5f);
        else
            draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_GradButtonHoveredD] : colors[ImGuiCol_GradButtonU]), height * 0.50f);
        draw_list->AddCircleFilled(ImVec2(p.x + radius + (*v ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, ColorConvertFloat4ToU32(  colors[ImGuiCol_CheckMark]));
        return true;
    }
}