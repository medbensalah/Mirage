#include "Sandbox2D.h"
#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.inl>

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f, true)
{
    quad2 = Mirage::Renderer2D::Primitives::Quad();
}

void Sandbox2D::OnAttach()
{
    MRG_PROFILE_FUNCTION();

    m_texture = Mirage::Texture2D::Create("assets/textures/CheckerBoard.png");
    quad.position = m_Position;
    quad.rotation = m_Rotation;
    quad.scale = m_Scale;
    quad.color = m_Color;

    quad2.position = m_Position;
    quad2.rotation = m_Rotation;
    quad2.scale = m_Scale;
    quad2.color = Vec4{0.1f, 0.35f, 0.9f, 1.0f};
}

void Sandbox2D::OnDetach()
{
    MRG_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(float DeltaTime)
{
    MRG_PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(DeltaTime);

    quad.position = m_Position;
    quad.rotation = m_Rotation;
    quad.scale = m_Scale;
    quad.color = m_Color;

    quad2.position = m_Position + 0.1f;
    quad2.rotation = Vec3(0.0f);
    quad2.scale = m_Scale;

    // Render
    {
        MRG_PROFILE_SCOPE("Renderer Clear");
        Mirage::RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
        Mirage::RenderCommand::Clear();
    }

    {
        MRG_PROFILE_SCOPE("Renderer Draw");
        Mirage::Renderer2D::BeginScene(m_CameraController.GetCamera());

            Mirage::Renderer2D::Draw::Quad(quad, m_texture, m_Tiling, m_Offset);
            Mirage::Renderer2D::Draw::Quad(quad2, m_texture, Vec2(3.0f));
        

        Mirage::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    MRG_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    MRG_IMGUI_DRAW_LABEL_WIDGET("Position", 75, ImGui::DragFloat3, "##Position", glm::value_ptr(m_Position), 0.05f);
    MRG_IMGUI_DRAW_LABEL_WIDGET("Rotation", 75, ImGui::DragFloat3, "##Rotation", glm::value_ptr(m_Rotation), 0.05f,
                                -180.0f, 180.0f);
    MRG_IMGUI_DRAW_LABEL_WIDGET("Scale", 75, ImGui::DragFloat3, "##Scale", glm::value_ptr(m_Scale), 0.05f);

    ImGui::Spacing();
    MRG_IMGUI_DRAW_LABEL_WIDGET("Color", 75, ImGui::ColorEdit4, "##Color", glm::value_ptr(m_Color));

    ImGui::Spacing();
    MRG_IMGUI_DRAW_LABEL_WIDGET("Tiling", 75, ImGui::DragFloat2, "##Tiling", glm::value_ptr(m_Tiling), 0.05f);
    MRG_IMGUI_DRAW_LABEL_WIDGET("Offset", 75, ImGui::DragFloat2, "##Offset", glm::value_ptr(m_Offset), 0.05f);

    ImGui::Spacing();
    ImGui::Separator();

    float deltaTime = Mirage::Application::Get().GetDeltaTime();

    ImGui::TextDisabled("FPS : %3.1f  (%f ms)", 1.0f / deltaTime, deltaTime * 1000.0f);
    ImGui::End();
}

void Sandbox2D::OnEvent(Mirage::Event& e)
{
    Layer::OnEvent(e);

    m_CameraController.OnEvent(e);
}
