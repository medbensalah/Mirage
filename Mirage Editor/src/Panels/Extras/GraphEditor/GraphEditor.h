#pragma once
#include <imgui_node_editor.h>

namespace Mirage::Graph
{
    namespace ed = ax::NodeEditor;

    class GraphEditor
    {
    public:
        GraphEditor();
        ~GraphEditor();

        void OnImGuiRender(bool* show);

    private:
        ed::EditorContext* m_Context = nullptr;
    };
}
