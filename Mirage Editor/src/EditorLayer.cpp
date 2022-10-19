#include "EditorLayer.h"
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.inl>

#include "ImGui/imgui_internal.h"
#include "Mirage/ECS/Components/CameraComponent.h"
#include "Mirage/ECS/Components/NativeScriptComponent.h"
#include "Mirage/ImGui/Extensions/GradientButtonV1.h"
#include "Mirage/ImGui/Extensions/ToggleButton.h"

#include "Mirage/ECS/Components/SpriteRendererComponent.h"
#include "Mirage/ECS/Components/TagComponent.h"
#include "Mirage/ECS/Components/TransformComponent.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"

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


    m_SquareEntitysec = m_ActiveScene->CreateSceneObject("Square2");
    m_SquareEntitysec.AddComponent<SpriteRendererComponent>( Vec4{1.0f,0.0f,0.0f,1.0f});
    m_SquareEntity = m_ActiveScene->CreateSceneObject("Square");
    m_SquareEntity.AddComponent<SpriteRendererComponent>( Vec4{0.0f,1.0f,0.0f,1.0f});

    m_Camera = m_ActiveScene->CreateSceneObject("Camera");
    m_Camera.AddComponent<CameraComponent>();

    class CameraController : public ScriptableSceneObject
    {
    private:
        float _speed = 5.0f;
        TransformComponent* _transform;
    public:
        void OnCreate()
        {
            _transform = &GetComponent<TransformComponent>();
        }

        void OnDestroy()
        {
            
        }

        void OnUpdate(float DeltaTime)
        {
            Vec3 position = _transform->Position();
            if(Input::IsKeyPressed(Key::A))
            {
                position.x -= _speed * DeltaTime;
            }
            if(Input::IsKeyPressed(Key::D))
            {
                position.x += _speed * DeltaTime;
            }
            if(Input::IsKeyPressed(Key::W))
            {
                position.y += _speed * DeltaTime;
            }
            if(Input::IsKeyPressed(Key::S))
            {
                position.y -= _speed * DeltaTime;
            }
            _transform->SetPosition(position);
        }
    };
    
    m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

    m_HierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::OnDetach()
{
    MRG_PROFILE_FUNCTION();
}

uint32_t max = 1000;

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
    if(m_ViewportFocused && m_ViewportHovered)
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
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.

            // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            // if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            // if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            // if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            // if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            // if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    ImGui::End();
}
void EditorLayer::CreateViewport()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::SetNextWindowClass(&s_ViewportWindowClass);
    ImGui::Begin("ViewPort");
    
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->AllowKbEvents(m_ViewportFocused && m_ViewportHovered);
    Application::Get().GetImGuiLayer()->AllowMouseEvents(m_ViewportHovered);
    
    ImVec2 ViewportSize = ImGui::GetContentRegionAvail();    
    m_ViewportSize = { ViewportSize.x, ViewportSize.y };
    uint32_t texId = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)texId,ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
    ImGui::PopStyleVar(1);
}
    
void EditorLayer::OnImGuiRender()
{
    MRG_PROFILE_FUNCTION();

    CreateDockspace();
    CreateViewport();

    m_HierarchyPanel.OnImguiRender();

    ImGui::Begin("Settings");
    
    // ImGui::ColoredButtonV1("You");
    //
    // 

    //
    // ImGui::Spacing();
    // MRG_IMGUI_DRAW_LABEL_WIDGET("Tiling", ImGui::DragFloat2, "##Tiling", glm::value_ptr(m_Tiling), 0.05f);
    // MRG_IMGUI_DRAW_LABEL_WIDGET("Offset", ImGui::DragFloat2, "##Offset", glm::value_ptr(m_Offset), 0.05f);
    //
    // ImGui::Spacing();
    // ImGui::Separator();

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
        return ImGui::ToggleButton("##ShowDemo",  &showDemo);
    });

    if(showDemo)
    {
        ImGui::ShowDemoWindow();
    }
    ImGui::End();

}

void EditorLayer::OnEvent(Event& e)
{
    m_CameraController.OnEvent(e);
}

}
