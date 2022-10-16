#pragma once

    namespace ImGui
    {
    static bool ToggleButton(const char* label, bool* v);
    }
    bool ImGui::ToggleButton(const char* label, bool* v)
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
        float width = height * 1.65f;
        float radius = height * 0.50f;
        float downScale = 0.7f;
        ImVec2 bgp ( p.x, p.y + height * (1-downScale) * 0.5f);
        
        ImGui::InvisibleButton(label, ImVec2(width, height));
        if (ImGui::IsItemClicked()) *v = !*v;
        ImGuiContext& gg = *GImGui;
        
        
        const ImGuiStyle& style = gg.Style;
        float ANIM_SPEED = 0.085f;
        const ImRect check_bb(bgp, ImVec2(bgp.x + width, bgp.y + height * downScale));
        ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
        
        if (label_size.x > 0.0f)
            RenderText(label_pos, label);
        if (gg.LastActiveId == gg.CurrentWindow->GetID(label))// && g.LastActiveIdTimer < ANIM_SPEED)
            float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
        if (ImGui::IsItemHovered())
            draw_list->AddRectFilled(bgp, ImVec2(bgp.x + width, bgp.y + height * downScale), ImGui::GetColorU32(colors[ImGuiCol_ButtonHovered]), height * 0.5f * downScale);
        else
            draw_list->AddRectFilled(bgp, ImVec2(bgp.x + width, bgp.y + height * downScale), ImGui::GetColorU32(*v ? colors[ImGuiCol_ButtonActive] : colors[ImGuiCol_GradButtonU]), height * 0.50f * downScale);
        draw_list->AddCircleFilled(ImVec2(p.x + radius + (*v ? 1.1f : -0.1f) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, ColorConvertFloat4ToU32(  colors[ImGuiCol_CheckMark]));
        return true;
    }
