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

    template <typename T, typename UIFUNCTION>
    static void DrawComponent(const char* name, SceneObject so, UIFUNCTION uiFunction)
    {
        if (!so.HasComponent<T>())
            return;

        auto& component = so.GetComponent<T>();
        static float indentWidth = 10.0f;

        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        
        const ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_SpanFullWidth |
            ImGuiTreeNodeFlags_AllowItemOverlap |
            ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_FramePadding;

        ImGui::Spacing();
        
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name);
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button(">", ImVec2(lineHeight, lineHeight)))
        {
            ImGui::OpenPopup("ComponentOptions");
        }
        
        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentOptions"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                removeComponent = true;
            }
            ImGui::EndPopup();
        }

        if (open)
        {
            ImGui::Spacing();
            ImGui::Indent(indentWidth);

            uiFunction(component);

            ImGui::TreePop();
            ImGui::Unindent(indentWidth);
        }

        if (removeComponent)
        {
            so.RemoveComponent<T>();
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
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Text("Tag");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.666f);

            bool dirty = ImGui::InputText("##Tag", buffer, sizeof(buffer));
            ImGui::SameLine();
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            if(ImGui::ButtonCenteredOnLine("Add", 1.0f,{ ImGui::GetContentRegionAvail().x , lineHeight}))
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
            
            if (dirty)
            {
                tag = std::string(buffer);
            }
            ImGui::Spacing();
        }
        
        DrawComponent<TransformComponent>("Transform", so,[](auto& component)
        {
            Vec3 position = component.Position();
                            Vec3 rotation = component.Rotation();
                            Vec3 scale = component.Scale();
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{7, 7});
                            if (
                                DrawSplitUIItem("Position", [&]()-> bool
                                    {
                                        return DrawVec3Control("Position", position);
                                    }, typeid(TransformComponent).name())
                                )
                            {
                                component.SetPosition(position);
                            }

                            if(
                                DrawSplitUIItem("Rotation", [&]()-> bool
                                    {
                                        return DrawVec3Control("Rotation",  rotation);
                                    }, typeid(TransformComponent).name())
                                )
                            {
                                component.SetRotation(rotation);
                            }

                            if(
                                DrawSplitUIItem("Scale", [&]()-> bool
                                    {
                                        return DrawVec3Control("Scale", scale);
                                    }, typeid(TransformComponent).name())
                                )
                            {
                                component.SetScale(scale);
                            }
                            ImGui::PopStyleVar();
        }
        );

        DrawComponent<CameraComponent>("Camera", so, [](auto& component)
                                       {
                                           auto& camera = component.Camera;

                                           const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
                                           const char* projectionTypeString = projectionTypeStrings[camera.
                                               GetProjectionType()];
                                           int out = (int)camera.GetProjectionType();

                                           if (DrawSplitUIItem("Projection", [&]()-> bool
                                           {
                                               return DrawComboBox("Projection", projectionTypeStrings, 2,
                                                                   projectionTypeString, &out);
                                           }, typeid(CameraComponent).name()))
                                           {
                                               camera.SetProjectionType(out);
                                           }

                                           if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                                           {
                                               float vFov = Degrees(camera.GetPerspectiveVFOV());
                                               if (DrawSplitUIItem("Vertical FOV", [&]()-> bool
                                               {
                                                   return ImGui::DragFloat(
                                                       "##Vertical FOV", &vFov, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
                                               }, typeid(CameraComponent).name()))
                                               {
                                                   camera.SetPerspectiveVFOV(Radians(vFov));
                                               }

                                               float nearClip = camera.GetPerspectiveNearClip();
                                               if (DrawSplitUIItem("near Clip", [&]()-> bool
                                               {
                                                   return ImGui::DragFloat(
                                                       "##near Clip", &nearClip, 0.1f, 0.1f, 1000.0f);
                                               }, typeid(CameraComponent).name()))
                                               {
                                                   camera.SetPerspectiveNearClip(nearClip);
                                               }

                                               float farClip = camera.GetPerspectiveFarClip();
                                               if (DrawSplitUIItem("Far Clip", [&]()-> bool
                                               {
                                                   return ImGui::DragFloat("##Far Clip", &farClip, 0.1f, 0.1f, 1000.0f);
                                               }, typeid(CameraComponent).name()))
                                               {
                                                   camera.SetPerspectiveFarClip(farClip);
                                               }
                                           }
                                           if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                                           {
                                               float size = camera.GetOrthographicSize();
                                               if (DrawSplitUIItem("Orthographic Size", [&]()-> bool
                                               {
                                                   return ImGui::DragFloat(
                                                       "##Orthographic Size", &size, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
                                               }, typeid(CameraComponent).name()))
                                               {
                                                   camera.SetOrthographicSize(size);
                                               }

                                               float nearClip = camera.GetOrthographicNearClip();
                                               if (DrawSplitUIItem("near Clip", [&]()-> bool
                                               {
                                                   return ImGui::DragFloat(
                                                       "##near Clip", &nearClip, 0.1f, 0.1f, 1000.0f);
                                               }, typeid(CameraComponent).name()))
                                               {
                                                   camera.SetOrthographicNearClip(nearClip);
                                               }

                                               float farClip = camera.GetOrthographicFarClip();
                                               if (DrawSplitUIItem("Far Clip", [&]()-> bool
                                               {
                                                   return ImGui::DragFloat("##Far Clip", &farClip, 0.1f, 0.1f, 1000.0f);
                                               }, typeid(CameraComponent).name()))
                                               {
                                                   camera.SetOrthographicFarClip(farClip);
                                               }
                                           }

                                           DrawSplitUIItem("Main", [&component]()-> bool
                                           {
                                               return ImGui::Checkbox("##Main", &component.IsMain);
                                           }, typeid(CameraComponent).name());

                                           if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                                           {
                                               DrawSplitUIItem("Fixed aspect ratio", [&component]()-> bool
                                               {
                                                   return ImGui::Checkbox("##FixedAspectRatio", &component.FixedAspectRatio);
                                               }, typeid(CameraComponent).name());
                                           }
                                       }
        );

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", so,[](auto& component)
        {
                DrawSplitUIItem("Color", [&component]()-> bool
                {
                    return ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color));
                }, typeid(SpriteRendererComponent).name());
        }
        );
    }
}
