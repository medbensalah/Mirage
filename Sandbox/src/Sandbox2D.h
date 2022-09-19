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

    Mirage::Ref<Mirage::VertexArray> m_SquareVA;
    Mirage::Ref<Mirage::Shader> m_FlatColorShader;

    Vec3 m_Position = { 0.0f, 0.0f, 0.0f };
    Vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
    Vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

    Vec4 m_Color = { 0.2f, 0.3f, 0.8f, 1.0f };
};
