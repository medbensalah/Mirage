#include "HierarchyPanel.h"

#include <ImGui/imgui.h>

#include "glm/gtc/type_ptr.hpp"
#include "ImGui/imgui_internal.h"

#include "Mirage/Core/Log.h"
#include "Mirage/ECS/Components/TagComponent.h"
#include "Mirage/ECS/Components/TransformComponent.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"

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
            SceneObject so{entityID, m_Context.get()};
            DrawEntityNode(so);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        ImGui::End();

        ImGui::Begin("Inspector");

        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }

        ImGui::End();
    }

    void HierarchyPanel::DrawEntityNode(SceneObject so)
    {
        auto& tag = so.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanFullWidth |
            // ImGuiTreeNodeFlags_Leaf |
            ((m_SelectionContext == so) ? ImGuiTreeNodeFlags_Selected : 0);
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)so, flags, tag.c_str());

        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = so;
        }

        if (opened)
        {
            ImGui::TreePop();
        }
    }

    void HierarchyPanel::DrawComponents(SceneObject so)
    {
        if (so.HasComponent<TagComponent>())
        {
            auto& tag = so.GetComponent<TagComponent>().Tag;
            
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            
            ImGui::Text("Tag");
            ImGui::SameLine();
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        if (so.HasComponent<TransformComponent>())
        {
            ImGuiTreeNodeFlags flags =
                ImGuiTreeNodeFlags_DefaultOpen |
                ImGuiTreeNodeFlags_CollapsingHeader;
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform"))
            {
                auto& tc = so.GetComponent<TransformComponent>();
                DrawSplitUIItem("Position", [&]()-> bool
                {
                    return ImGui::DragFloat3("##Position", glm::value_ptr(tc.Transform[3]), 0.1f);
                });
            }
        }
    }
}
