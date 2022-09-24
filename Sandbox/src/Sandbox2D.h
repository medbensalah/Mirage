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
    
    Mirage::Renderer2D::Primitives::Quad quad;
    Mirage::Renderer2D::Primitives::Quad quad2;
    
    Vec3 m_Position = { 0.0f, 0.0f, 0.0f };
    Vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
    Vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

    Mirage::Ref<Mirage::Texture2D> m_texture;
    
    Vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    Vec2 m_Tiling = { 1.0f, 1.0f };
    Vec2 m_Offset = { 0.0f, 0.0f };
};
