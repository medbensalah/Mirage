#include "GraphEditor.h"
#include <ImGui/imgui.h>

namespace Mirage::Graph
{
    GraphEditor::GraphEditor()
    {
        ed::Config config;
        config.SettingsFile = "GraphEditor.json";
        m_Context = ed::CreateEditor(&config);
    }

    GraphEditor::~GraphEditor()
    {
        if (m_Context)
        {
            ed::DestroyEditor(m_Context);
            m_Context = nullptr;
        }
    }

    void GraphEditor::OnImGuiRender(bool* show)
    {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        if (ImGui::Begin("Graph Editor", show, flags))
        {
            ed::SetCurrentEditor(m_Context);
            ed::Begin("My Node Editor");
            
            // Example Node
            ed::BeginNode(1);
            ImGui::Text("Node A");
            ed::BeginPin(1, ed::PinKind::Input);
            ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ed::BeginPin(2, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();
            ed::EndNode();

            ed::End();
            ed::SetCurrentEditor(nullptr);
        }
        ImGui::End();
    }
}
