#pragma once

#include "Mirage.h"

class Sandbox2D : public Mirage::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(float DeltaTime) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Mirage::Event& e) override;
private:
    Mirage::OrthographicCameraController m_CameraController;
    
    Vec3 m_Position = { 0.0f, 0.0f, 0.0f };
    Vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
    Vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

    Mirage::Ref<Mirage::Texture2D> m_texture;
    
    Vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    Vec2 m_Tiling = { 1.0f, 1.0f };
    Vec2 m_Offset = { 0.0f, 0.0f };

    float m_DeltaTime = 0.0f;

    struct ProfileResult
    {
        const char* Name;
        float Time;
    };

    std::vector<ProfileResult> m_ProfileResults;

    Mirage::Renderer2D::Primitives::Quad quad{m_Position, m_Rotation, m_Scale, m_Color, m_texture, m_Tiling, m_Offset};

};
