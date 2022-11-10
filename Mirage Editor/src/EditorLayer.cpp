﻿#if 0
#include "EditorLayer.h"

#include <filesystem>
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.inl>

#include "glm/ext/scalar_common.hpp"
#include "glm/gtx/log_base.hpp"
#include "glm/gtx/transform2.hpp"
#include "glm/gtx/vector_angle.inl"
#include "Mirage/ECS/Components/CameraComponent.h"
#include "Mirage/ECS/Components/NativeScriptComponent.h"
#include "Mirage/ImGui/Extensions/GradientButtonV1.h"
#include "Mirage/ImGui/Extensions/ToggleButton.h"

#include "Mirage/ECS/Components/SpriteRendererComponent.h"
#include "Mirage/ECS/Components/TagComponent.h"
#include "Mirage/ECS/Components/TransformComponent.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"

#include "Mirage/ECS/SceneSerializer.h"
#include "Mirage/Events/KeyEvent.h"
#include "Mirage/Math/Math.h"
#include "Mirage/utils/PlatformUtils.h"

namespace Mirage
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
    {
    }

    static ImGuiWindowClass s_ViewportWindowClass;

    void EditorLayer::OnAttach()
    {
        MRG_PROFILE_FUNCTION();
        s_ViewportWindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

        m_texture = Texture2D::Create("assets/textures/CheckerBoard.png");

        FramebufferSpecs specs;
        specs.Width = 1280;
        specs.Height = 720;
        m_Framebuffer = Framebuffer::Create(specs);

        m_ActiveScene = CreateRef<Scene>();

#if OLD_PATH
        m_SquareEntity = m_ActiveScene->CreateSceneObject("Square 1");
        m_SquareEntity.AddComponent<SpriteRendererComponent>(Vec4{1.0f, 0.0f, 0.0f, 1.0f});

        // for(int i =0; i< 90000; i++)
        // {
        //
        //     m_SquareEntity = m_ActiveScene->CreateSceneObject("Square 2");
        //     m_SquareEntity.AddComponent<SpriteRendererComponent>( Vec4{0.0f,1.0f,0.0f,1.0f});
        // }

        m_Camera = m_ActiveScene->CreateSceneObject("Camera");
        m_Camera.AddComponent<CameraComponent>();

        class CameraController : public ScriptableSceneObject
        {
        private:
            float _speed = 5.0f;
        public:
            void OnCreate()
            {
            }

            void OnDestroy()
            {
            }

            void OnUpdate(float DeltaTime)
            {
                auto& _transform = GetComponent<TransformComponent>();
                Vec3 position = _transform.Position();
                if (Input::IsKeyPressed(Key::A))
                {
                    position.x -= _speed * DeltaTime;
                }
                if (Input::IsKeyPressed(Key::D))
                {
                    position.x += _speed * DeltaTime;
                }
                if (Input::IsKeyPressed(Key::W))
                {
                    position.y += _speed * DeltaTime;
                }
                if (Input::IsKeyPressed(Key::S))
                {
                    position.y -= _speed * DeltaTime;
                }
                _transform.SetPosition(position);
            }
        };

        m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

        m_HierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
        MRG_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(float DeltaTime)
    {
        MRG_PROFILE_FUNCTION();
        // MRG_TRACE("Frame time : {0:.3f} ms   -   FPS : {1:.1f}", DeltaTime * 1000.0f, 1.0f / DeltaTime);
        // MRG_TRACE("Total elapsed time : {0:.3f} s", Application::GetSeconds());


        // Resize
        if (FramebufferSpecs spec = m_Framebuffer->GetSpecs();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }


        // Update
        if (m_ViewportFocused && m_ViewportHovered)
        {
            m_CameraController.OnUpdate(DeltaTime);
        }


        // Render
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
        RenderCommand::Clear();

        m_ActiveScene->OnUpdate(DeltaTime);

        m_Framebuffer->Unbind();
    }

    static bool showDemo = false;

    void EditorLayer::CreateDockspace()
    {
        MRG_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {300.0f, 25.0f});
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        ImGui::PopStyleVar();

        CreateMenuBar();
        
        ImGui::End();
    }
    void EditorLayer::CreateMenuBar()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl + N"))
                    NewScene();


                if (ImGui::MenuItem("Open...", "Ctrl + O"))
                    OpenScene();

                // TODO: Save
                
                if (ImGui::MenuItem("Save as...", "Ctrl + shift + S"))
                    SaveAs();
                
                
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        
    }
    void EditorLayer::CreateViewport()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::SetNextWindowClass(&s_ViewportWindowClass);
        ImGui::Begin("ViewPort");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        bool IsAnyImGuiActive = ImGui::IsAnyItemActive();
        Application::Get().GetImGuiLayer()->AllowKbEvents((m_ViewportFocused || m_ViewportHovered) && !IsAnyImGuiActive);
        Application::Get().GetImGuiLayer()->AllowMouseEvents(m_ViewportHovered && IsAnyImGuiActive);

        ImVec2 ViewportSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = {ViewportSize.x, ViewportSize.y};
        uint32_t texId = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)texId, ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
        
        // Gizmos
        SceneObject selectedSO = m_HierarchyPanel.GetSelectedSO();
        if (selectedSO)
        {
            ImGuizmo::SetOrthographic(false);
            // ImGuizmo::AllowAxisFlip(false);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            // TODO : change to editor camera
            auto CameraSO = m_ActiveScene->GetMainCameraSO();
            if (CameraSO)
            {
                const auto& camera = CameraSO.GetComponent<CameraComponent>().Camera;
                const Mat4 cameraProjection = camera.GetProjection();
                Mat4 CameraView = Inverse(CameraSO.GetComponent<TransformComponent>().GetTransform());

                auto& tc = selectedSO.GetComponent<TransformComponent>();

                Mat4 transform = tc.GetTransform();
                Mat4 delta;

                // Snapping
                
                float snapValues[3];
                
                bool snap = Input::IsKeyPressed(Key::LeftControl);
                if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
                    snapValues[0] = snapValues[1] = snapValues[2] = 0.50f;
                if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                    snapValues[0] = snapValues[1] = snapValues[2] = 15.0f;
                if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
                    snapValues[0] = snapValues[1] = snapValues[2] = 0.25f;
                    

                
                bool GizmoAction;
                 
                // Scale only locally
                if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
                {
                    GizmoAction = ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(cameraProjection),
                                         ImGuizmo::OPERATION::SCALE, ImGuizmo::LOCAL,
                                         glm::value_ptr(transform), glm::value_ptr(delta),
                                         snap ? snapValues : nullptr);
                }
                else
                {
                    GizmoAction = ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(cameraProjection),
                                         m_GizmoType, m_GizmoMode,
                                         glm::value_ptr(transform), glm::value_ptr(delta),
                                         snap ? snapValues : nullptr);
                }

                if (GizmoAction)
                {
                    Vec3 position, rotation, scale;

                    Math::DecomposeTransform(delta, position, rotation, scale);
                    rotation = Degrees(rotation);
                    
                    switch (m_GizmoType)
                    {
                    case ImGuizmo::OPERATION::TRANSLATE:
                        tc.SetWorldPosition(tc.WorldPosition() + position);
                        break;
                    case ImGuizmo::OPERATION::ROTATE:
                        tc.Rotate(rotation);
                        break;
                    case ImGuizmo::OPERATION::SCALE:
                        tc.Scale(scale);
                        break;
                    }
                }
            }
        }
        
        ImGui::End();
        ImGui::PopStyleVar(1);
    }
    void EditorLayer::CreateToolBar()
    {
        ImGuiWindowFlags toolbarFlags = ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoTitleBar
        | ImGuiNextWindowDataFlags_HasSizeConstraint;
        ImGui::SetNextWindowSizeConstraints({ImGui::GetContentRegionAvail().x, 25.0f}, {ImGui::GetContentRegionAvail().x, 25.0f});
        
        ImGui::Begin("ToolBar", nullptr, toolbarFlags);


        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImVec2 label_size = ImGui::CalcTextSize("Space : ", NULL, true);
        const ImVec2 pos = ImGui::GetCursorPos();
        ImVec2 label_pos = ImVec2(pos.x, pos.y + style.FramePadding.y);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Space : ");
        ImGui::SameLine();
        
        bool IsLocalSpace = m_GizmoMode == ImGuizmo::LOCAL;
        bool change;
        if (!IsLocalSpace)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            change = ImGui::Button("W", ImVec2{30.0f, 25.0f});
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            change |= ImGui::Button("L", ImVec2{30.0f, 25.0f});
        }
        else
        {
            change = ImGui::Button("W", ImVec2{30.0f, 25.0f});
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            change |= ImGui::Button("L", ImVec2{30.0f, 25.0f});
            ImGui::PopStyleColor(3);
        }
        if (change)
        {
            m_GizmoMode = m_GizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.30f, 0.30f, 0.30f, 1.000f));
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::Text("Mode : ");
        ImGui::SameLine();

        bool isTranslating = m_GizmoType == ImGuizmo::OPERATION::TRANSLATE;
        bool isRotating = m_GizmoType == ImGuizmo::OPERATION::ROTATE;
        bool isScaling = m_GizmoType == ImGuizmo::OPERATION::SCALE;
        
        if (isTranslating)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            if (ImGui::Button("T", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = (ImGuizmo::OPERATION)-1;
            }
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            if (ImGui::Button("R", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("S", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            }
        }
        else if (isRotating)
        {
            if (ImGui::Button("T", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            if (ImGui::Button("R", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = (ImGuizmo::OPERATION)-1;
            }
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            if (ImGui::Button("S", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            }
        }
        else if (isScaling)
        {
            if (ImGui::Button("T", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("R", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            if (ImGui::Button("S", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = (ImGuizmo::OPERATION)-1;
            }
            ImGui::PopStyleColor(3);
        }
        else
        {
            if (ImGui::Button("T", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("R", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("S", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = (ImGuizmo::OPERATION)-1;
            }            
        }

        
        ImGui::End();
    }

    
    void EditorLayer::OnImGuiRender()
    {
        MRG_PROFILE_FUNCTION();

        CreateDockspace();
        CreateViewport();
        CreateToolBar();

        m_HierarchyPanel.OnImguiRender();

        ImGui::Begin("Settings");

        float deltaTime = Application::Get().GetDeltaTime();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Indent();
        ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
        ImGui::Text("Quads: %d", Renderer2D::GetStats().QuadCount);
        ImGui::Text("Vertices: %d", Renderer2D::GetStats().GetVertexCount());
        ImGui::Text("Indices: %d", Renderer2D::GetStats().GetIndexCount());
        ImGui::Unindent();
        ImGui::Spacing();
        ImGui::TextDisabled("FPS : %3.1f  (%f ms)", 1.0f / deltaTime, deltaTime * 1000.0f);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        DrawSplitUIItem("Show demo", [&]()-> bool
        {
            return ImGui::ToggleButton("##ShowDemo", &showDemo);
        });

        if (showDemo)
        {
            ImGui::ShowDemoWindow();
        }
        ImGui::End();
    }
    
    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);

        // Shortcuts
        ProcessShortcuts(e);
    }
    
    void EditorLayer::ProcessShortcuts(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(MRG_BIND_EVENT_FN(EditorLayer::OnShortcutKeyPressed));
    }

    bool EditorLayer::OnShortcutKeyPressed(KeyPressedEvent e)
    {
        if (e.GetRepeatCount() > 0)
            return false;

        bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        // --------------------------------- Menu ---------------------------------
        if (ctrl)
        {
            switch (e.GetKeyCode())
            {
            case Key::N:
                NewScene();
                break;
            case Key::O:
                OpenScene();
                break;
            case Key::S:
                SaveAs();
                break;
            default:
                break;
            }
        }
        // --------------------------------- Gizmos ---------------------------------

        else
        {
            switch (e.GetKeyCode())
            {
            case Key::Q:
                m_GizmoType = (ImGuizmo::OPERATION)-1;
                break;
            case Key::W:
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case Key::E:
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            case Key::R:
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            case Key::M:
                m_GizmoMode = m_GizmoMode == ImGuizmo::MODE::LOCAL ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;
                break;
            default:
                break;
            }
        }
    }


    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_HierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
        const std::string filepath = FileDialogs::OpenFile("Mirage scene (*.mrgs)\0*.mrgs\0");
        
        if(!filepath.empty())
        {
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_HierarchyPanel.SetContext(m_ActiveScene);
                        
            SceneSerializer serializer(m_ActiveScene);
            serializer.DeserializeText(filepath);
        }        
    }

    void EditorLayer::SaveAs()
    {
        std::string filepath = FileDialogs::SaveFile("Mirage scene (*.mrgs)\0*.mrgs\0");
        if(!filepath.empty())
        {
            // check for extension
            if (std::filesystem::path(filepath).extension() != ".mrgs")
            {
                filepath.append(".mrgs");
            }
            SceneSerializer serializer(m_ActiveScene);
            serializer.SerializeText(filepath);
        }
    }
}
#else
#include "EditorLayer.h"

#include <filesystem>
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.inl>

#include "glm/ext/scalar_common.hpp"
#include "glm/gtx/log_base.hpp"
#include "glm/gtx/transform2.hpp"
#include "glm/gtx/vector_angle.inl"
#include "Mirage/ECS/Components/CameraComponent.h"
#include "Mirage/ECS/Components/NativeScriptComponent.h"
#include "Mirage/ImGui/Extensions/GradientButtonV1.h"
#include "Mirage/ImGui/Extensions/ToggleButton.h"

#include "Mirage/ECS/Components/SpriteRendererComponent.h"
#include "Mirage/ECS/Components/TagComponent.h"
#include "Mirage/ECS/Components/TransformComponent.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"

#include "Mirage/ECS/SceneSerializer.h"
#include "Mirage/Events/KeyEvent.h"
#include "Mirage/Math/Math.h"
#include "Mirage/utils/PlatformUtils.h"

namespace Mirage
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
    {
    }

    static ImGuiWindowClass s_ViewportWindowClass;

    void EditorLayer::OnAttach()
    {
        MRG_PROFILE_FUNCTION();
        s_ViewportWindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

        m_texture = Texture2D::Create("assets/textures/CheckerBoard.png");

        FramebufferSpecs specs;
        specs.Width = 1280;
        specs.Height = 720;
        m_Framebuffer = Framebuffer::Create(specs);

        m_ActiveScene = CreateRef<Scene>();

#if OLD_PATH
        m_SquareEntity = m_ActiveScene->CreateSceneObject("Square 1");
        m_SquareEntity.AddComponent<SpriteRendererComponent>(Vec4{1.0f, 0.0f, 0.0f, 1.0f});

        // for(int i =0; i< 90000; i++)
        // {
        //
        //     m_SquareEntity = m_ActiveScene->CreateSceneObject("Square 2");
        //     m_SquareEntity.AddComponent<SpriteRendererComponent>( Vec4{0.0f,1.0f,0.0f,1.0f});
        // }

        m_Camera = m_ActiveScene->CreateSceneObject("Camera");
        m_Camera.AddComponent<CameraComponent>();

        class CameraController : public ScriptableSceneObject
        {
        private:
            float _speed = 5.0f;
        public:
            void OnCreate()
            {
            }

            void OnDestroy()
            {
            }

            void OnUpdate(float DeltaTime)
            {
                auto& _transform = GetComponent<TransformComponent>();
                Vec3 position = _transform.Position();
                if (Input::IsKeyPressed(Key::A))
                {
                    position.x -= _speed * DeltaTime;
                }
                if (Input::IsKeyPressed(Key::D))
                {
                    position.x += _speed * DeltaTime;
                }
                if (Input::IsKeyPressed(Key::W))
                {
                    position.y += _speed * DeltaTime;
                }
                if (Input::IsKeyPressed(Key::S))
                {
                    position.y -= _speed * DeltaTime;
                }
                _transform.SetPosition(position);
            }
        };

        m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

        m_HierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
        MRG_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(float DeltaTime)
    {
        MRG_PROFILE_FUNCTION();
        // MRG_TRACE("Frame time : {0:.3f} ms   -   FPS : {1:.1f}", DeltaTime * 1000.0f, 1.0f / DeltaTime);
        // MRG_TRACE("Total elapsed time : {0:.3f} s", Application::GetSeconds());


        // Resize
        if (FramebufferSpecs spec = m_Framebuffer->GetSpecs();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }


        // Update
        if (m_ViewportFocused && m_ViewportHovered)
        {
            m_CameraController.OnUpdate(DeltaTime);
        }


        // Render
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
        RenderCommand::Clear();

        m_ActiveScene->OnUpdate(DeltaTime);

        m_Framebuffer->Unbind();
    }

    static bool showDemo = false;

    void EditorLayer::CreateDockspace()
    {
        MRG_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {300.0f, 25.0f});
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        ImGui::PopStyleVar();

        CreateMenuBar();
        
        ImGui::End();
    }
    void EditorLayer::CreateMenuBar()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl + N"))
                    NewScene();


                if (ImGui::MenuItem("Open...", "Ctrl + O"))
                    OpenScene();

                // TODO: Save
                
                if (ImGui::MenuItem("Save as...", "Ctrl + shift + S"))
                    SaveAs();
                
                
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        
    }

    
    void EditorLayer::CreateViewport()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::SetNextWindowClass(&s_ViewportWindowClass);
        ImGui::Begin("ViewPort");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        bool IsAnyImGuiActive = ImGui::IsAnyItemActive();
        Application::Get().GetImGuiLayer()->AllowKbEvents((m_ViewportFocused || m_ViewportHovered) && !IsAnyImGuiActive);
        Application::Get().GetImGuiLayer()->AllowMouseEvents(m_ViewportHovered && IsAnyImGuiActive);

        ImVec2 ViewportSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = {ViewportSize.x, ViewportSize.y};
        uint32_t texId = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)texId, ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
        
        // Gizmos
        SceneObject selectedSO = m_HierarchyPanel.GetSelectedSO();
        if (selectedSO)
        {
            ImGuizmo::SetOrthographic(false);
            // ImGuizmo::AllowAxisFlip(false);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            // TODO : change to editor camera
            auto CameraSO = m_ActiveScene->GetMainCameraSO();
            if (CameraSO)
            {
                ImGuizmo::AllowAxisFlip(false);
                const auto& camera = CameraSO.GetComponent<CameraComponent>().Camera;
                const Mat4 cameraProjection = camera.GetProjection();
                Mat4 CameraView = Inverse(CameraSO.GetComponent<TransformComponent>().GetTransform());

                auto& tc = selectedSO.GetComponent<TransformComponent>();

                Mat4 transform = tc.GetTransform();
                Mat4 delta;

                // Snapping
                
                float snapValues[3];

                bool snap = false;
                if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
                {
                    snap = m_TranslationSnap;
                    snapValues[0] = snapValues[1] = snapValues[2] = m_TranslationSnapValue;
                }
                if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                {
                    snap = m_RotationSnap;
                    snapValues[0] = snapValues[1] = snapValues[2] = m_RotationSnapValue;
                }
                if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
                {
                    snap = m_ScaleSnap;
                    snapValues[0] = snapValues[1] = snapValues[2] = m_ScaleSnapValue;
                }
                
                snap |= Input::IsKeyPressed(Key::LeftControl);
                
                bool GizmoAction;
                 
                if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
                {
                    GizmoAction = ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(cameraProjection),
                                         ImGuizmo::OPERATION::SCALE, ImGuizmo::LOCAL,
                                         glm::value_ptr(transform), glm::value_ptr(delta),
                                         snap ? snapValues : nullptr);
                }
                else
                {
                    GizmoAction = ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(cameraProjection),
                                         m_GizmoType, m_GizmoMode,
                                         glm::value_ptr(transform), glm::value_ptr(delta),
                                         snap ? snapValues : nullptr);
                }
                if (GizmoAction)
                {
                    Vec3 position, rotation, scale;
                    Math::DecomposeTransform(delta, position, rotation, scale);
                    rotation = Degrees(rotation);

                    switch (m_GizmoType)
                    {
                    case ImGuizmo::OPERATION::TRANSLATE:
                        tc.SetWorldPosition(tc.WorldPosition() + position);
                        break;
                    case ImGuizmo::OPERATION::ROTATE:
                        tc.Rotate(rotation);
                        break;
                    case ImGuizmo::OPERATION::SCALE:
                        Math::DecomposeTransform(transform, position, rotation, scale);
                        tc.SetWorldScale(scale);
                        break;
                    }
                }
            }
        }
        
        ImGui::End();
        ImGui::PopStyleVar(1);
    }
    void EditorLayer::CreateToolBar()
    {
        ImGuiWindowFlags toolbarFlags = ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoTitleBar
        | ImGuiNextWindowDataFlags_HasSizeConstraint;
        ImGui::SetNextWindowSizeConstraints({ImGui::GetContentRegionAvail().x, 25.0f}, {ImGui::GetContentRegionAvail().x, 25.0f});
        
        ImGui::Begin("ToolBar", nullptr, toolbarFlags);

        // -------------------------- Spacing --------------------------
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImVec2 label_size = ImGui::CalcTextSize("Space : ", NULL, true);
        const ImVec2 pos = ImGui::GetCursorPos();
        ImVec2 label_pos = ImVec2(pos.x, pos.y + style.FramePadding.y);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Space : ");
        ImGui::SameLine();
        
        bool IsLocalSpace = m_GizmoMode == ImGuizmo::LOCAL;
        bool change;
        if (!IsLocalSpace)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            change = ImGui::Button("W", ImVec2{30.0f, 25.0f});
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            change |= ImGui::Button("L", ImVec2{30.0f, 25.0f});
        }
        else
        {
            change = ImGui::Button("W", ImVec2{30.0f, 25.0f});
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            change |= ImGui::Button("L", ImVec2{30.0f, 25.0f});
            ImGui::PopStyleColor(3);
        }
        if (change)
        {
            m_GizmoMode = m_GizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
        }
        
        // -------------------------- Mode --------------------------

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.30f, 0.30f, 0.30f, 1.000f));
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 17);
        ImGui::Text("Mode : ");
        ImGui::SameLine();

        bool isTranslating = m_GizmoType == ImGuizmo::OPERATION::TRANSLATE;
        bool isRotating = m_GizmoType == ImGuizmo::OPERATION::ROTATE;
        bool isScaling = m_GizmoType == ImGuizmo::OPERATION::SCALE;
        
        if (isTranslating)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            if (ImGui::Button("T", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = (ImGuizmo::OPERATION)-1;
            }
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            if (ImGui::Button("R", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("S", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            }
            ImGui::SameLine();
        }
        else if (isRotating)
        {
            if (ImGui::Button("T", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            if (ImGui::Button("R", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = (ImGuizmo::OPERATION)-1;
            }
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            if (ImGui::Button("S", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            }
            ImGui::SameLine();
        }
        else if (isScaling)
        {
            if (ImGui::Button("T", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("R", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
            if (ImGui::Button("S", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = (ImGuizmo::OPERATION)-1;
            }
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
        }
        else
        {
            if (ImGui::Button("T", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("R", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            ImGui::SameLine();
            if (ImGui::Button("S", ImVec2{30.0f, 25.0f}))
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            }
            ImGui::SameLine();
        }

        // ------------------ Snapping ------------------

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.30f, 0.30f, 0.30f, 1.000f));
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 17);
        ImGui::Text("Snapping : ");
        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, style.ItemSpacing.y});
        ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.000f, 0.000f, 0.490f, 1.000f));
        
        ImGui::Text("T: ");
        ImGui::SameLine();
        ImGui::Checkbox("##TranslationSnap", &m_TranslationSnap);
        ImGui::SameLine();
        ImGui::PushItemWidth(50);
        ImGui::DragFloat("##TranslationSnapping", &m_TranslationSnapValue, 0.1f, 0.0f, 1000.0f, "%.3g");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12);
        
        ImGui::Text("R: ");
        ImGui::SameLine();
        ImGui::Checkbox("##RotationSnap", &m_RotationSnap);
        ImGui::SameLine();
        ImGui::PushItemWidth(50);
        ImGui::DragFloat("##RotationSnapping", &m_RotationSnapValue, 1.0f, 0.0f, 180.0f, "%.2g");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12);
        
        ImGui::Text("S: ");
        ImGui::SameLine();
        ImGui::Checkbox("##ScaleSnap", &m_ScaleSnap);
        ImGui::SameLine();
        ImGui::PushItemWidth(50);
        ImGui::DragFloat("##ScaleSnapping", &m_ScaleSnapValue, 0.01f, 0.0f, 1.0f, "%.3g");
        ImGui::PopItemWidth();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12);

        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        
        ImGui::End();
    }

    
    void EditorLayer::OnImGuiRender()
    {
        MRG_PROFILE_FUNCTION();

        CreateDockspace();
        CreateViewport();
        CreateToolBar();

        m_HierarchyPanel.OnImguiRender();

        ImGui::Begin("Settings");

        float deltaTime = Application::Get().GetDeltaTime();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Indent();
        ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
        ImGui::Text("Quads: %d", Renderer2D::GetStats().QuadCount);
        ImGui::Text("Vertices: %d", Renderer2D::GetStats().GetVertexCount());
        ImGui::Text("Indices: %d", Renderer2D::GetStats().GetIndexCount());
        ImGui::Unindent();
        ImGui::Spacing();
        ImGui::TextDisabled("FPS : %3.1f  (%f ms)", 1.0f / deltaTime, deltaTime * 1000.0f);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        DrawSplitUIItem("Show demo", [&]()-> bool
        {
            return ImGui::ToggleButton("##ShowDemo", &showDemo);
        });

        if (showDemo)
        {
            ImGui::ShowDemoWindow();
        }
        ImGui::End();
    }
    
    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);

        // Shortcuts
        ProcessShortcuts(e);
    }
    
    void EditorLayer::ProcessShortcuts(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(MRG_BIND_EVENT_FN(EditorLayer::OnShortcutKeyPressed));
    }

    bool EditorLayer::OnShortcutKeyPressed(KeyPressedEvent e)
    {
        if (e.GetRepeatCount() > 0)
            return false;

        bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        // --------------------------------- Menu ---------------------------------
        if (ctrl)
        {
            switch (e.GetKeyCode())
            {
            case Key::N:
                NewScene();
                break;
            case Key::O:
                OpenScene();
                break;
            case Key::S:
                SaveAs();
                break;
            default:
                break;
            }
        }
        // --------------------------------- Gizmos ---------------------------------

        else
        {
            switch (e.GetKeyCode())
            {
            case Key::Q:
                m_GizmoType = (ImGuizmo::OPERATION)-1;
                break;
            case Key::W:
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case Key::E:
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            case Key::R:
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            case Key::M:
                m_GizmoMode = m_GizmoMode == ImGuizmo::MODE::LOCAL ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;
                break;
            default:
                break;
            }
        }
    }


    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_HierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
        const std::string filepath = FileDialogs::OpenFile("Mirage scene (*.mrgs)\0*.mrgs\0");
        
        if(!filepath.empty())
        {
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_HierarchyPanel.SetContext(m_ActiveScene);
                        
            SceneSerializer serializer(m_ActiveScene);
            serializer.DeserializeText(filepath);
        }        
    }

    void EditorLayer::SaveAs()
    {
        std::string filepath = FileDialogs::SaveFile("Mirage scene (*.mrgs)\0*.mrgs\0");
        if(!filepath.empty())
        {
            // check for extension
            if (std::filesystem::path(filepath).extension() != ".mrgs")
            {
                filepath.append(".mrgs");
            }
            SceneSerializer serializer(m_ActiveScene);
            serializer.SerializeText(filepath);
        }
    }
}

#endif