#pragma once

namespace Mirage
{
    static bool DrawSplitUIItem(const char* label, std::function<bool()> fn)
    {
        ImGui::Columns(2);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
        bool result = fn();
        ImGui::PopItemWidth();
        ImGui::Columns(1);
        return result;
    }
}
