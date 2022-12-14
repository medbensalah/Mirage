#include "HierarchyPanel.h"

#include <ImGui/imgui.h>

#include "glm/gtc/type_ptr.hpp"

#include "Mirage/Core/Log.h"
#include "Mirage/Definitions/Debug.h"
#include "Mirage/ECS/Components/Rendering/CameraComponent.h"
#include "Mirage/ECS/Components/Rendering/SpriteRendererComponent.h"
#include "Mirage/ECS/Components/Base/TagComponent.h"
#include "Mirage/ECS/Components/Base/TransformComponent.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"
#include "Mirage/ImGui/Extensions/ButtonExtensions.h"
#include "Mirage/Definitions/DragnDropPayloads.h"
#include "Mirage/ECS/Components/Physics/BoxCollider2DComponent.h"
#include "Mirage/ECS/Components/Physics/RigidBody2DComponent.h"

namespace Mirage
{
    HierarchyPanel::HierarchyPanel(const Ref<Scene>& scene)
    {
        SetContext(scene);
    }

    void HierarchyPanel::SetContext(const Ref<Scene>& scene)
    {
        m_SelectionContext = {};
        m_Context = scene;
    }

    void HierarchyPanel::SetSelectedSO(SceneObject so, bool ignoreHierarchy)
    {
        if (so)
        {
            if (m_SelectionContext == so)
            {
                if (m_SelectionContext.GetParent() && !ignoreHierarchy)
                {
	                m_SelectionContext = m_SelectionContext.GetParent();
                }
                return;
            }
        }
        
        m_SelectionContext = so;
    }

    void HierarchyPanel::OnImGuiRender()
    {
    	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        if (ImGui::Begin("Outliner"))
        {
	        if (Debug::HierarchyDebug)
	        {
		        ImGui::BeginChild("Debug Scene Hierarchy", ImVec2(0, 100), true);
	        	ImGui::Text("hierarchy size  : %d", m_Context->m_Hierarchy.size());
	        	
	        	ImGui::Separator();

	        	
	        	if (m_SelectionContext)
	        	{
		        	auto& h = m_SelectionContext.GetComponent<HierarchyComponent>();
	        		ImGui::Text("children size  : %d", h.m_Children.size());
	        		ImGui::Text("childrenset size  : %d", h.m_ChildrenSet.size());
	        		ImGui::Text("index : %d", h.m_Index);
	        		ImGui::Text("entity : %d", h.m_entity);        			
	        	}
	        	ImGui::EndChild();

	        	ImGui::Separator();
	        }
        	
        	
	        for (auto& h : m_Context->m_Hierarchy)
			{
				SceneObject so {h.second.m_entity, m_Context.get()};
				if(!so.HasParent())
				{
					DrawEntityNode(so);
				}
			};

        	if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
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
        }
        ImGui::End();
	    ImGui::PopStyleVar();


    	if (ImGui::Begin("Inspector"))
        {
	        if (m_SelectionContext)
	        {
	        	DrawComponents(m_SelectionContext);
	        }
        }
        ImGui::End();
    }

    void HierarchyPanel::DrawEntityNode(SceneObject so)
    {
        auto& tag = so.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_SpanFullWidth |
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
                m_Context->CreateChildSceneObject(so);
            if(ImGui::MenuItem("Duplicate"))
	            SetSelectedSO(m_Context->DuplicateSceneObject(so));
            
            ImGui::EndPopup();
        }

    	// TODO: Drag and drop
    	// ImGui::InvisibleButton("##drag", ImVec2(ImGui::GetContentRegionAvail().x, 3.0f));

        if(ImGui::IsKeyPressed(ImGuiKey_Delete))
        {
            if(m_SelectionContext == so)
                entityDeleted = true;
        }
        
        if (opened)
        {
	        auto v = &so.GetComponent<HierarchyComponent>().m_ChildrenSet;
	        for (auto child : *v)
	        {
		        DrawEntityNode({child.m_entity, m_Context.get()});
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
            
            char buffer[256] = {};
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

            if (ImGui::BeginPopup("AddComponent"))
            {
	            if (!m_SelectionContext.HasComponent<CameraComponent>())
	            {
		            if (ImGui::MenuItem("Camera"))
		            {
			            m_SelectionContext.AddComponent<CameraComponent>();
			            ImGui::CloseCurrentPopup();
		            }
                }
                if(!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                {
	                if(ImGui::MenuItem("Sprite Renderer"))
	                {
	                	m_SelectionContext.AddComponent<SpriteRendererComponent>();
	                	ImGui::CloseCurrentPopup();
	                }
                }
                if(!m_SelectionContext.HasComponent<RigidBody2DComponent>())
                {
	                if(ImGui::MenuItem("Rigid body 2D"))
	                {
	                	m_SelectionContext.AddComponent<RigidBody2DComponent>();
	                	ImGui::CloseCurrentPopup();
	                }
                }
                if(!m_SelectionContext.HasComponent<BoxCollider2DComponent>())
                {
	                if(ImGui::MenuItem("Box collider 2D"))
	                {
	                	m_SelectionContext.AddComponent<BoxCollider2DComponent>();
	                	ImGui::CloseCurrentPopup();
	                }
                }
                ImGui::EndPopup();
            }

            if (dirty)
            {
	            tag = std::string(buffer);
            }
            ImGui::Spacing();
        }

		DrawComponent<TransformComponent>("Transform", so, [](auto& component)
			{
    			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{7, 7});
				Vec3 position = component.Position();
				Vec3 rotation = component.Rotation();
				Vec3 scale = component.Scale();
				if (
					DrawSplitUIItem("Position", [&]()-> bool
					{
						return DrawVec3Control("Position", position);
					}, typeid(TransformComponent).name())
				)
				{
					component.SetPosition(position);
				}

				if (
					DrawSplitUIItem("Rotation", [&]()-> bool
					{
						return DrawVec3Control("Rotation", rotation);
					}, typeid(TransformComponent).name())
				)
				{
					component.SetRotation(rotation);
				}

				if (
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
				const char* projectionTypeString = projectionTypeStrings[camera.GetProjectionType()];
				int out = (int)camera.GetProjectionType();

				if (DrawSplitUIItem("Projection", [&]()-> bool
				{
					return DrawComboBox("##Projection", projectionTypeStrings, 2,projectionTypeString, &out);
				}, typeid(CameraComponent).name()))
				{
					camera.SetProjectionType(out);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float vFov = Degrees(camera.GetPerspectiveVFOV());
					if (DrawSplitUIItem("Vertical FOV", [&]()-> bool
					{
						return ImGui::DragFloat("##Vertical FOV", &vFov, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
					}, typeid(CameraComponent).name()))
					{
						camera.SetPerspectiveVFOV(Radians(vFov));
					}

					float nearClip = camera.GetPerspectiveNearClip();
					if (DrawSplitUIItem("near Clip", [&]()-> bool
					{
						return ImGui::DragFloat("##near Clip", &nearClip, 0.1f, 0.1f, 1000.0f);
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
						return ImGui::DragFloat("##Orthographic Size", &size, 0.1f, 0.1f, 1000.0f, "%.1f", 1.0f);
					}, typeid(CameraComponent).name()))
					{
						camera.SetOrthographicSize(size);
					}

					float nearClip = camera.GetOrthographicNearClip();
					if (DrawSplitUIItem("near Clip", [&]()-> bool
					{
						return ImGui::DragFloat("##near Clip", &nearClip, 0.1f, 0.1f, 1000.0f);
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
	        	Vec4 cc = camera.GetClearColor();
        		if(DrawSplitUIItem("Clear color", [&]()-> bool
					{
						return ImGui::ColorEdit4("##ClearColor", glm::value_ptr(cc));
					}, typeid(CameraComponent).name()))
        		{
        			camera.SetClearColor(cc);
        		}
			}
        );

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", so, [&so](auto& component)
			{
				DrawSplitUIItem("Color", [&component]()-> bool
				{
					return ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color));
		        }, typeid(SpriteRendererComponent).name());
        	
				static float size = 128.0f * ImGui::GetIO().FontGlobalScale;
        	
        		DrawSplitUIItem("Preview size", []()-> bool
				{
					return ImGui::DragFloat("##PreviewSize", &size, 1.0f, 16.0f, ImGui::GetContentRegionAvail().x, "%.3g");
				}, typeid(SpriteRendererComponent).name());
				bool textureChange = DrawSplitUIItem("Texture", [&component, &so]()-> bool
				{
					float middlePos = (ImGui::GetContentRegionAvail().x - size) / 2.0f;
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + middlePos);
					bool pressed = ImGui::ImageButton((ImTextureID)component.Texture->GetRendererID(), {size, size},
	                    {0, 1}, {1, 0}, 0,
	                    {0, 0, 0, 0}, {component.Color.r, component.Color.g, component.Color.b, component.Color.a}
	                    );
					return pressed;
				}, typeid(SpriteRendererComponent).name());
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Payloads::texture.c_str()))
						{
							char path[512] = {};
							memcpy(path, payload->Data, payload->DataSize);
							Ref<Texture2D> texture = Texture2D::Create(path);
							if (texture->IsLoaded())
								component.Texture = texture;
							else
								MRG_WARN("Could not load texture {0}", path);
						}
						ImGui::EndDragDropTarget();
					}
        		
				DrawSplitUIItem("Tiling", [&component]()-> bool
				{
					return ImGui::DragFloat2("##Tiling", glm::value_ptr(component.Tiling), 0.01f, 0.0f, 0.0f, "%.4g");
				}, typeid(SpriteRendererComponent).name());
				DrawSplitUIItem("Offset", [&component]()-> bool
				{
					return ImGui::DragFloat2("##Offset", glm::value_ptr(component.Offset), 0.01f, 0.0f, 0.0f, "%.4g");
				}, typeid(SpriteRendererComponent).name());
			}
        );
    	
        DrawComponent<RigidBody2DComponent>("Rigidbody 2D", so, [&so](auto& component)
			{
				const char* bodyTypeStrings[] = {"Static", "Kinematic", "Dynamic"};
				const char* bodyTypeString = bodyTypeStrings[(int)component.Type];
				int out = (int)component.Type;

				if (DrawSplitUIItem("Body type", [&]()-> bool
				{
					return DrawComboBox("##BodyType", bodyTypeStrings, 3,bodyTypeString, &out);
				}, typeid(RigidBody2DComponent).name()))
				{
					component.Type = (RigidBody2DComponent::BodyType)out;
				}
        	        		
				DrawSplitUIItem("Gravity scale", [&component]()-> bool
				{
					return ImGui::DragFloat("##GravityScale", &component.GravityScale, 0.1f, 0.0f, 10000.0f, "%.3g");
		        }, typeid(RigidBody2DComponent).name());
        	
				DrawSplitUIItem("Fixed rotation Z", [&component]()-> bool
				{
					return ImGui::Checkbox("##FixedRotationZ", &component.FixedRotation);
				}, typeid(RigidBody2DComponent).name());
			}
        );
    	
        DrawComponent<BoxCollider2DComponent>("BoxCollider 2D", so, [&so](auto& component)
			{
				DrawSplitUIItem("Size", [&component]()-> bool
				{
					return ImGui::DragFloat2("##Size", glm::value_ptr(component.Size), 0.05f, 0, 0, "%.4g");
		        }, typeid(RigidBody2DComponent).name());
        	
				DrawSplitUIItem("Offset", [&component]()-> bool
				{
					return ImGui::DragFloat2("##Offset", glm::value_ptr(component.Offset), 0.1f, 0, 0, "%.4g");
		        }, typeid(RigidBody2DComponent).name());
        	
				DrawSplitUIItem("Density", [&component]()-> bool
				{
					return ImGui::DragFloat("##Density", &component.Density, 0.05f, 0, 0, "%.4g");
		        }, typeid(RigidBody2DComponent).name());
				DrawSplitUIItem("Friction", [&component]()-> bool
				{
					return ImGui::DragFloat("##Friction", &component.Friction, 0.05f, 0.0f, 1.0f, "%.4g");
		        }, typeid(RigidBody2DComponent).name());
				DrawSplitUIItem("Bounciness", [&component]()-> bool
				{
					return ImGui::DragFloat("##Bounciness", &component.Bounciness, 0.05f, 0.0f, 1.0f, "%.4g");
		        }, typeid(RigidBody2DComponent).name());
				DrawSplitUIItem("BouncinessThreshold", [&component]()-> bool
				{
					return ImGui::DragFloat("##BouncinessThreshold", &component.BouncinessThreshold, 0.05f, 0.0f, 1.0f, "%.4g");
		        }, typeid(RigidBody2DComponent).name());
			}
        );
    }
}
