#include "Sandbox2D.h"
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.inl>

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
    m_texture = Mirage::Texture2D::Create("assets/textures/CheckerBoard.png");
}

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(float DeltaTime)
{
    MRG_PROFILE_FUNCTION();
    // Update
    {
        MRG_PROFILE_SCOPE("CameraController::OnUpdate");
        m_CameraController.OnUpdate(DeltaTime);
    }
    
    Mirage::Renderer2D::Primitives::Quad quad{m_Position, m_Rotation, m_Scale, m_Color, m_texture, m_Tiling, m_Offset};
    
    // Render
    {
        MRG_PROFILE_SCOPE("Renderer Clear");
        Mirage::RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
        Mirage::RenderCommand::Clear();
    }
    
    {
        MRG_PROFILE_SCOPE("Renderer Draw");
        Mirage::Renderer2D::BeginScene(m_CameraController.GetCamera());

        Mirage::Renderer2D::Draw::Quad(quad);

        Mirage::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    MRG_PROFILE_FUNCTION();
    
    ImGui::Begin("Settings");
    ImGui::DragFloat3("Position", glm::value_ptr(m_Position), 0.05f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(m_Rotation), 0.05f, -180.0f, 180.0f);
    ImGui::DragFloat3("Scale", glm::value_ptr(m_Scale), 0.05f);

    ImGui::Spacing();
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));

    ImGui::Spacing();
    ImGui::DragFloat2("Tiling", glm::value_ptr(m_Tiling), 0.05f);
    ImGui::DragFloat2("Offset", glm::value_ptr(m_Offset), 0.05f);

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::End();
}

void Sandbox2D::OnEvent(Mirage::Event& e)
{
    Layer::OnEvent(e);

    m_CameraController.OnEvent(e);
}
