#include "HierarchyPanel.h"

#include <ImGui/imgui.h>

#include "glm/gtc/type_ptr.hpp"

#include "Mirage/Core/Log.h"
#include "Mirage/ECS/Components/CameraComponent.h"
#include "Mirage/ECS/Components/SpriteRendererComponent.h"
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
        static float indentWidth = 10.0f;
        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_CollapsingHeader;
        
        if (so.HasComponent<TagComponent>())
        {
            auto& tag = so.GetComponent<TagComponent>().Tag;
            
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            
            ImGui::Text("Tag");
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        if (so.HasComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), flags, "Transform"))
            {
            ImGui::Indent(indentWidth);
                auto& tc = so.GetComponent<TransformComponent>();
                
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{7, 7
                });
                DrawSplitUIItem("Position", [&]()-> bool
                {
                    return DrawVec3Control("Position", tc.Position);
                }, typeid(TransformComponent).name());
                
                DrawSplitUIItem("Rotation", [&]()-> bool
                {
                    return DrawVec3Control("Rotation", tc.Rotation);
                }, typeid(TransformComponent).name());
                
                DrawSplitUIItem("Scale", [&]()-> bool
                {
                    return DrawVec3Control("Scale", tc.Scale);
                }, typeid(TransformComponent).name());
                ImGui::PopStyleVar();
                
            ImGui::Unindent(indentWidth);
            }
        }
        
        if (so.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), flags, "Camera"))
            {
                ImGui::Indent(indentWidth);
                auto& cc = so.GetComponent<CameraComponent>();
                auto& camera = so.GetComponent<CameraComponent>().Camera;
                
                const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
                const char* projectionTypeString = projectionTypeStrings[camera.GetProjectionType()];
                int out = (int)camera.GetProjectionType();

                if(DrawSplitUIItem("Projection", [&]()->bool
                {
                    return DrawComboBox("Projection", projectionTypeStrings, 2, projectionTypeString, &out);
                }, typeid(CameraComponent).name()))
                {
                    camera.SetProjectionType(out);
                }
                
                if(camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    float vFov = Degrees(camera.GetPerspectiveVFOV());
                    if (DrawSplitUIItem("Vertical FOV", [&]()-> bool
                    {
                        return ImGui::DragFloat("##Vertical FOV", &vFov, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
                    },  typeid(CameraComponent).name()))
                    {
                        camera.SetPerspectiveVFOV(Radians(vFov));
                    }
            
                    float nearClip = camera.GetPerspectiveNearClip();
                    if (DrawSplitUIItem("near Clip", [&]()-> bool
                    {
                        return ImGui::DragFloat("##near Clip", &nearClip, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
                    },  typeid(CameraComponent).name()))
                    {
                        camera.SetPerspectiveNearClip(nearClip);
                    }
            
                    float farClip = camera.GetPerspectiveFarClip();
                    if (DrawSplitUIItem("Far Clip", [&]()-> bool
                    {
                        return ImGui::DragFloat("##Far Clip", &farClip, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
                    },  typeid(CameraComponent).name()))
                    {
                        camera.SetPerspectiveFarClip(farClip);
                    }
                }
                if(camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    float size = camera.GetOrthographicSize();
                    if (DrawSplitUIItem("Orthographic Size", [&]()-> bool
                    {
                        return ImGui::DragFloat("##Orthographic Size", &size, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
                    },  typeid(CameraComponent).name()))
                    {
                        camera.SetOrthographicSize(size);
                    }
            
                    float nearClip = camera.GetOrthographicNearClip();
                    if (DrawSplitUIItem("near Clip", [&]()-> bool
                    {
                        return ImGui::DragFloat("##near Clip", &nearClip, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
                    },  typeid(CameraComponent).name()))
                    {
                        camera.SetOrthographicNearClip(nearClip);
                    }
            
                    float farClip = camera.GetOrthographicFarClip();
                    if (DrawSplitUIItem("Far Clip", [&]()-> bool
                    {
                        return ImGui::DragFloat("##Far Clip", &farClip, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
                    },  typeid(CameraComponent).name()))
                    {
                        camera.SetOrthographicFarClip(farClip);
                    }
                }
            
                DrawSplitUIItem("Main", [&cc]()->bool
                {
                    return ImGui::Checkbox("##Main", &cc.IsMain);
                },  typeid(CameraComponent).name());
                
                if(camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    DrawSplitUIItem("Fixed aspect ratio", [&cc]()->bool
                    {
                        return ImGui::Checkbox("##FixedAspectRatio", &cc.FixedAspectRatio);
                    },  typeid(CameraComponent).name());
                }
               
                ImGui::Unindent(indentWidth);
            }
        }

        if(so.HasComponent<SpriteRendererComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), flags, "Sprite renderer"))
            {
                ImGui::Indent(indentWidth);
                
                auto& src = so.GetComponent<SpriteRendererComponent>();
                DrawSplitUIItem("Color", [&]()-> bool
                {
                    return ImGui::ColorEdit4("##Color", glm::value_ptr(src.Color));
                }, typeid(SpriteRendererComponent).name());
                
                ImGui::Unindent(indentWidth);
            }
        }
    }
}
