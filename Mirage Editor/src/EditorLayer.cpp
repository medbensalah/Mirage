#include "EditorLayer.h"
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.inl>

#include "ImGui/imgui_internal.h"

namespace Mirage
{
EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
{
    quad2 = Renderer2D::Primitives::Quad();
}

void EditorLayer::OnAttach()
{
    MRG_PROFILE_FUNCTION();

    m_texture = Texture2D::Create("assets/textures/CheckerBoard.png");
    quad.position = m_Position;
    quad.rotation = m_Rotation;
    quad.scale = m_Scale;
    quad.color = m_Color;
    quad.texture = m_texture;

    quad2.position = m_Position;
    quad2.rotation = m_Rotation;
    quad2.scale = m_Scale;
    quad2.color = Vec4{0.1f, 0.35f, 0.9f, 1.0f};

    FramebufferSpecs specs;
    specs.Width = 1280;
    specs.Height = 720;
    m_Framebuffer = Framebuffer::Create(specs);
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
    // Update
    if(m_ViewportFocused && m_ViewportHovered)
    {
        m_CameraController.OnUpdate(DeltaTime);
    }

    quad.position = m_Position;
    quad.rotation = m_Rotation;
    quad.scale = m_Scale;
    quad.color = m_Color;

    quad2.position = m_Position + 0.1f;
    quad2.rotation = Vec3(0.0f);
    quad2.scale = m_Scale;
    

    // Render
    Renderer2D::ResetStats();
    {
        MRG_PROFILE_SCOPE("Renderer Clear");
        m_Framebuffer->Bind();
        // RenderCommand::SetClearColor({1.0f,1.0f,1.0f,0.0f});
        RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
        RenderCommand::Clear();
    }

    {
        MRG_PROFILE_SCOPE("Scene 1");
        Renderer2D::BeginScene(m_CameraController.GetCamera());
        
        Renderer2D::Draw::Quad(quad, m_Tiling, m_Offset);

        
        for(float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for(float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                Vec2 color = {(x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f};
                Renderer2D::Draw::Quad(
                    {
                        Vec3(x, y, 0.0f),
                        Vec3(0.0f),
                        Vec3(0.45f),
                        {color.x, color.y,1.0f, 1.0f}
                    });
            }
        }
    
        Renderer2D::EndScene();
        m_Framebuffer->Unbind();
    }
}

static bool showDemo = true;

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
    ImGui::Begin("ViewPort");

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->AllowKbEvents(m_ViewportFocused && m_ViewportHovered);
    Application::Get().GetImGuiLayer()->AllowMouseEvents(m_ViewportHovered);
    
    ImVec2 ViewportSize = ImGui::GetContentRegionAvail();
    if(m_ViewportSize != *((glm::vec2*)&ViewportSize))
    {
        m_Framebuffer->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
        m_ViewportSize = {ViewportSize.x, ViewportSize.y};
        m_CameraController.OnResize(ViewportSize.x, ViewportSize.y);
    }
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
    



    
    
    ImGui::Begin("Settings");
    
    MRG_IMGUI_DRAW_LABEL_WIDGET("Position", 95, ImGui::DragFloat3, "##Position", glm::value_ptr(m_Position), 0.05f);
    MRG_IMGUI_DRAW_LABEL_WIDGET("Rotation", 95, ImGui::DragFloat3, "##Rotation", glm::value_ptr(m_Rotation), 0.05f, -180.0f, 180.0f);
    MRG_IMGUI_DRAW_LABEL_WIDGET("Scale", 95, ImGui::DragFloat3, "##Scale", glm::value_ptr(m_Scale), 0.05f);
    
    ImGui::Spacing();
    MRG_IMGUI_DRAW_LABEL_WIDGET("Color", 95, ImGui::ColorEdit4, "##Color", glm::value_ptr(m_Color));
    
    ImGui::Spacing();
    MRG_IMGUI_DRAW_LABEL_WIDGET("Tiling", 95, ImGui::DragFloat2, "##Tiling", glm::value_ptr(m_Tiling), 0.05f);
    MRG_IMGUI_DRAW_LABEL_WIDGET("Offset", 95, ImGui::DragFloat2, "##Offset", glm::value_ptr(m_Offset), 0.05f);
    
    ImGui::Spacing();
    ImGui::Separator();

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
    
    MRG_IMGUI_DRAW_LABEL_WIDGET("Show demo", 95, ImGui::Checkbox, "##Showdemo", &showDemo);
    if(showDemo)
    {
        ImGui::ShowDemoWindow(&showDemo);
    }
    ImGui::End();

}

void EditorLayer::OnEvent(Event& e)
{
    m_CameraController.OnEvent(e);
}

}
