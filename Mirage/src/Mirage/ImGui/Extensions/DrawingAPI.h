#pragma once

namespace Mirage
{
    static float width = 60.0f;
    static bool DrawSplitUIItem(const char* label, std::function<bool()> fn, const char* id = "tab");

    static bool DrawComboBox(const char* label, const char* strings[], int count, const char* preview, int* out, std::function<void()> fn);
}

bool Mirage::DrawSplitUIItem(const char* label, std::function<bool()> fn, const char* id)
{
    bool result = false;
    if(ImGui::BeginTable(id, 2, ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
        ImGui::TableNextRow();
        for (int column = 0; column < 2; column++)
        {
            ImGui::TableSetColumnIndex(column);
            if (column == 0)
            {
                ImGui::Text(label);
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

bool Mirage::DrawComboBox(const char* label, const char* strings[], int count, const char* preview, int* out, std::function<void()> fn)
{
    bool result = false;
    
    if (ImGui::BeginCombo(label, preview))
    {
        for(int i = 0; i < 2; i++)
        {
            bool isSelected = preview == strings[i];
            
            if (ImGui::Selectable(strings[i], isSelected))
            {
                preview = strings[i];
                *out = i;
                fn();
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