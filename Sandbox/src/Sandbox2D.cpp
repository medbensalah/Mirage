﻿#include "Sandbox2D.h"
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
    quad.color = m_Color;
    quad.texture = m_texture;

    quad2.color = Vec4{0.1f, 0.35f, 0.9f, 1.0f};

}

void Sandbox2D::OnDetach()
{
    MRG_PROFILE_FUNCTION();
}

uint32_t max = 1000;

Mat4 tr = MatTranslate(Mat4(1.0f), Vec3(0.0f, 0.0f, 0.0f)) * MatScale(Mat4(1.0f), Vec3(0.25f, 0.25f, 1.0f));

void Sandbox2D::OnUpdate(float DeltaTime)
{
    MRG_PROFILE_FUNCTION();

    MRG_TRACE("Frame time : {0:.3f} ms   -   FPS : {1:.1f}", DeltaTime * 1000.0f, 1.0f / DeltaTime);
    // MRG_TRACE("Total elapsed time : {0:.3f} s", Mirage::Application::GetSeconds());
    // Update
    m_CameraController.OnUpdate(DeltaTime);
    

    // Render
    Mirage::Renderer2D::ResetStats();
    {
        MRG_PROFILE_SCOPE("Renderer Clear");
        
        // Mirage::RenderCommand::SetClearColor({0.0f,0.0f,0.0f,1.0f});
        Mirage::RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
        Mirage::RenderCommand::Clear();
    }

    {
        MRG_PROFILE_SCOPE("Scene 1");
        Mirage::Renderer2D::BeginScene(m_CameraController.GetCamera());
        
        Mirage::Renderer2D::Draw::Quad(quad);

    	
        
        for(float y = -10.0f; y < 10.0f; y += 0.5f)
        {
            for(float x = -10.0f; x < 10.0f; x += 0.5f)
            {
                Vec4 color = { (x + 10.0f) / 20.0f, 0.4f, (y + 10.0f) / 20.0f, 1.0f };
                Mirage::Renderer2D::Draw::Quad(MatTranslate(tr, Vec3(x * 3.0f, y * 3.0f, 0.0f)), color);
                // Mirage::Renderer2D::Draw::Quad(glm::translate(quad.transform, Vec3(x, y, 0.0f)), color, m_texture);
            }
        }
        
        Mirage::Renderer2D::EndScene();
    }
}

static bool showDemo = true;

void Sandbox2D::OnImGuiRender()
{
    MRG_PROFILE_FUNCTION();
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

    float deltaTime = Mirage::Application::Get().GetDeltaTime();
    ImGui::TextDisabled("FPS : %3.1f  (%f ms)", 1.0f / deltaTime, deltaTime * 1000.0f);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::End();
}

void Sandbox2D::OnEvent(Mirage::Event& e)
{
    m_CameraController.OnEvent(e);
}
