#include "Sandbox2D.h"
#include "ImGui/imgui.h"
#include "glm/gtc/type_ptr.inl"
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
: Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
    Layer::OnAttach();

    
}

void Sandbox2D::OnDetach()
{
    Layer::OnDetach();
}

void Sandbox2D::OnUpdate(float DeltaTime)
{
    Layer::OnUpdate(DeltaTime);
    // Update
    m_CameraController.OnUpdate(DeltaTime);

    // Render
    Mirage::RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
    Mirage::RenderCommand::Clear();

    Mirage::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Mirage::Renderer2D::Draw::Quad(m_Position, m_Rotation,m_Scale, m_Color);
    Mirage::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    Layer::OnImGuiRender();

    ImGui::Begin("Settings");
    ImGui::DragFloat3("Position", glm::value_ptr(m_Position), 0.05f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(m_Rotation),0.05f,-180.0f,180.0f);
    ImGui::DragFloat3("Scale", glm::value_ptr(m_Scale), 0.05f);
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));
    ImGui::End();
}

void Sandbox2D::OnEvent(Mirage::Event& e)
{
    Layer::OnEvent(e);

    m_CameraController.OnEvent(e);
}
