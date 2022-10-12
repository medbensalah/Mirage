#include "HierarchyPanel.h"

#include <ImGui/imgui.h>

#include "Mirage/Core/Log.h"
#include "Mirage/ECS/Components/TagComponent.h"

namespace Mirage
{
    HierarchyPanel::HierarchyPanel(const Ref<Scene>& scene)
    {
        SetContext(scene);
    }

    void HierarchyPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void HierarchyPanel::OnImguiRender()
    {
        ImGui::Begin("Outliner");

        m_Context->m_Registry.each([&](auto entityID)
        {
            SceneObject so{ entityID, m_Context.get() };
            DrawEntityNode(so);
            
        });
        
        ImGui::End();
    }

    void HierarchyPanel::DrawEntityNode(SceneObject so)
    {
        auto& tag = so.GetComponent<TagComponent>().Tag;
        
        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanFullWidth |
            ((m_SelectionContext == so) ? ImGuiTreeNodeFlags_Selected : 0);
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)so, flags, tag.c_str());

        if(ImGui::IsItemClicked())
        {
            m_SelectionContext = so;
        }

        if(opened)
        {
            ImGui::TreePop();
        }

    }
}
