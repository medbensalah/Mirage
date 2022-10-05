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
    quad.texture = m_texture;

    quad2.position = m_Position;
    quad2.rotation = m_Rotation;
    quad2.scale = m_Scale;
    quad2.color = Vec4{0.1f, 0.35f, 0.9f, 1.0f};

}

void Sandbox2D::OnDetach()
{
    MRG_PROFILE_FUNCTION();
}

uint32_t max = 1000;

void Sandbox2D::OnUpdate(float DeltaTime)
{
    MRG_PROFILE_FUNCTION();

    // MRG_TRACE("Frame time : {0:.3f} ms   -   FPS : {1:.1f}", DeltaTime * 1000.0f, 1.0f / DeltaTime);
    // MRG_TRACE("Total elapsed time : {0:.3f} s", Mirage::Application::GetSeconds());
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
    Mirage::Renderer2D::ResetStats();
    {
        MRG_PROFILE_SCOPE("Renderer Clear");
        
        Mirage::RenderCommand::SetClearColor({1.0f,1.0f,1.0f,0.0f});
        // Mirage::RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
        Mirage::RenderCommand::Clear();
    }

    {
        MRG_PROFILE_SCOPE("Scene 1");
        Mirage::Renderer2D::BeginScene(m_CameraController.GetCamera());
        
        Mirage::Renderer2D::Draw::Quad(quad, m_Tiling, m_Offset);

        
        for(float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for(float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                Vec2 color = {(x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f};
                Mirage::Renderer2D::Draw::Quad(
                    {
                        Vec3(x, y, 0.0f),
                        Vec3(0.0f),
                        Vec3(0.45f),
                        {color.x, color.y,1.0f, 1.0f}
                    });
            }
        }
        
        Mirage::Renderer2D::EndScene();
    }
}

static bool showDemo = true;

void Sandbox2D::OnImGuiRender()
{
    MRG_PROFILE_FUNCTION();
}

void Sandbox2D::OnEvent(Mirage::Event& e)
{
    m_CameraController.OnEvent(e);
}
