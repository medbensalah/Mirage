#pragma once
#include <iomanip>
#include <sstream>

#include "ImGui/imgui_internal.h"
#include "Mirage/ImGui/FontLibrary.h"


namespace Mirage
{
    static float width = 60.0f;
    static bool DrawSplitUIItem(const char* label, std::function<bool()> fn, const char* id = "tab");
    static bool DrawComboBox(const char* label, const char* strings[], int count, const char* preview, int* out);

    static bool DrawVec3Control(const char* label, Vec3& vector);
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
bool Mirage::DrawComboBox(const char* label, const char* strings[], int count, const char* preview, int* out)
{
    bool result = false;
    if (ImGui::BeginCombo(label, preview))
    {
        for (int i = 0; i < 2; i++)
        {
            bool isSelected = preview == strings[i];

            if (ImGui::Selectable(strings[i], isSelected))
            {
                preview = strings[i];
                *out = i;
                // fn();
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
    
    // TODO: formatting
    static std::string buffer;
    static std::string t;
    static std::string fmt;
    static std::stringstream out;

    out.str(std::string());
    out << vector.x;
    buffer = out.str();
    t = buffer.substr(buffer.find(".")+1);
    fmt = "%." + std::to_string(t.length()) + "f";
    
    ImGui::PushID("X");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{-4, 3});
    ImGui::PushStyleColor(ImGuiCol_Button, col1);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col1);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, col1);
    ImGui::PushFont(blackFont);
    ImGui::Button("X", buttonSize);
    ImGui::PopFont();
    ImGui::SameLine();
    result |= ImGui::DragFloat("##X", &vector.x, 0.1f, 0.0f, 0.0f, fmt.c_str());
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    ImGui::SameLine();
    ImGui::PopItemWidth();
    ImGui::PopID();
    
    out.str(std::string());
    out << vector.y;
    buffer = out.str();
    t = buffer.substr(buffer.find(".")+1);
    fmt = "%." + std::to_string(t.length()) + "f";
    
    ImGui::PushID("Y");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{-4, 3});
    ImGui::PushStyleColor(ImGuiCol_Button, col2);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col2);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, col2);
    ImGui::PushFont(blackFont);
    ImGui::Button("Y", buttonSize);
    ImGui::PopFont();
    ImGui::SameLine();
    result |= ImGui::DragFloat("##Y", &vector.y, 0.1f, 0.0f, 0.0f, fmt.c_str());
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    ImGui::SameLine();
    ImGui::PopItemWidth();
    ImGui::PopID();
    
    out.str(std::string());

    out << vector.z;
    buffer = out.str();
    t = buffer.substr(buffer.find(".")+1);
    fmt = "%." + std::to_string(t.length()) + "f";
    
    ImGui::PushID("Z");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{-4, 3});
    ImGui::PushStyleColor(ImGuiCol_Button, col3);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col3);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, col3);
    ImGui::PushFont(blackFont);
    ImGui::Button("Z", buttonSize);
    ImGui::PopFont();
    ImGui::SameLine();
    result |= ImGui::DragFloat("##Z", &vector.z, 0.1f, 0.0f, 0.0f, fmt.c_str());
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    ImGui::SameLine();
    ImGui::PopItemWidth();
    ImGui::PopID();
        
    ImGui::PopID();

    ImGui::EndGroup();

    return result;
}
