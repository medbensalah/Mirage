#include "HierarchyPanel.h"

#include <ImGui/imgui.h>

#include "glm/gtc/type_ptr.hpp"

#include "Mirage/Core/Log.h"
#include "Mirage/ECS/Components/CameraComponent.h"
#include "Mirage/ECS/Components/SpriteRendererComponent.h"
#include "Mirage/ECS/Components/TagComponent.h"
#include "Mirage/ECS/Components/TransformComponent.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"
#include "Mirage/ImGui/Extensions/GradientButtonV1.h"

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
            if(!so.HasParent())
            {
                DrawEntityNode(so);
            }
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};
        
        //Right-Click on blank space
        if(ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if(ImGui::MenuItem("Create empty SceneObject"))
            {
                m_Context->CreateSceneObject("new SceneObject");
            }

            ImGui::EndPopup();
        }
        
        ImGui::End();
        
        ImGui::Begin("Inspector");

        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();        
            ImGui::Spacing();        
            ImGui::Spacing();        
            if(ImGui::ButtonCenteredOnLine("Add Component", 0.5f,{ImGui::GetWindowWidth()-50, 30}))
            {
                ImGui::OpenPopup("AddComponent");
            }

            if(ImGui::BeginPopup("AddComponent"))
            {
                if(ImGui::MenuItem("Camera"))
                {
                    if(!m_SelectionContext.HasComponent<CameraComponent>())
                        m_SelectionContext.AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }
                if(ImGui::MenuItem("Sprite Renderer"))
                {
                    if(!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                        m_SelectionContext.AddComponent<SpriteRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }

    void HierarchyPanel::DrawEntityNode(SceneObject so)
    {
        auto& tag = so.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_SpanFullWidth |
            ImGuiTreeNodeFlags_OpenOnArrow |
            (so.GetChildCount() ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf) |
            ((m_SelectionContext == so) ? ImGuiTreeNodeFlags_Selected : 0);

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)so, flags, tag.c_str());

        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = so;
        }

        bool entityDeleted = false;
        
        
        if(ImGui::BeginPopupContextItem())
        {
            if(ImGui::MenuItem("Delete SceneObject"))
                entityDeleted = true;
            if(ImGui::MenuItem("Add Child SceneObject"))
            {
                m_Context->CreateChildSceneObject(so);
            }
            
            ImGui::EndPopup();
        }

        if(ImGui::IsKeyPressed(ImGuiKey_Delete))
        {
            if(m_SelectionContext == so)
                entityDeleted = true;
        }
        
        if (opened)
        {
            // bool opened2 = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
            // if(opened2)
            //     ImGui::TreePop();

                auto v = &m_Context->m_Hierarchy.at(so).m_Children;
                for(auto child : *v)
                {
                    DrawEntityNode({child, m_Context.get()});
                }
            
            ImGui::TreePop();
        }

        if(entityDeleted)
        {
            m_Context->DestroySceneObject(so);
            if(m_SelectionContext == so)
                m_SelectionContext = {};
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
                ImGui::Spacing();
                ImGui::Indent(indentWidth);
                auto& tc = so.GetComponent<TransformComponent>();
                Vec3 position = tc.Position();
                Vec3 rotation = tc.Rotation();
                Vec3 scale = tc.Scale();
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{7, 7});
                if (
                    DrawSplitUIItem("Position", [&]()-> bool
                        {
                            return DrawVec3Control("Position", position);
                        }, typeid(TransformComponent).name())
                    )
                {
                    tc.SetPosition(position);
                }

                if(
                    DrawSplitUIItem("Rotation", [&]()-> bool
                        {
                            return DrawVec3Control("Rotation",  rotation);
                        }, typeid(TransformComponent).name())
                    )
                {
                    tc.SetRotation(rotation);
                }

                if(
                    DrawSplitUIItem("Scale", [&]()-> bool
                        {
                            return DrawVec3Control("Scale", scale);
                        }, typeid(TransformComponent).name())
                    )
                {
                    tc.SetScale(scale);
                }
                ImGui::PopStyleVar();

                ImGui::Unindent(indentWidth);
            }
        }
        
        if (so.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), flags, "Camera"))
            {
                ImGui::Spacing();
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
                        return ImGui::DragFloat("##near Clip", &nearClip, 0.1f, 0.1f, 1000.0f);
                    },  typeid(CameraComponent).name()))
                    {
                        camera.SetPerspectiveNearClip(nearClip);
                    }
            
                    float farClip = camera.GetPerspectiveFarClip();
                    if (DrawSplitUIItem("Far Clip", [&]()-> bool
                    {
                        return ImGui::DragFloat("##Far Clip", &farClip, 0.1f, 0.1f, 1000.0f);
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
                        return ImGui::DragFloat("##near Clip", &nearClip, 0.1f, 0.1f, 1000.0f);
                    },  typeid(CameraComponent).name()))
                    {
                        camera.SetOrthographicNearClip(nearClip);
                    }
            
                    float farClip = camera.GetOrthographicFarClip();
                    if (DrawSplitUIItem("Far Clip", [&]()-> bool
                    {
                        return ImGui::DragFloat("##Far Clip", &farClip, 0.1f, 0.1f, 1000.0f);
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
                ImGui::Spacing();
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
