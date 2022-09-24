#pragma once

#include <Mirage.h>
#include <glm/gtc/type_ptr.inl>
#include <ImGui/imgui.h>

class ExampleLayer : public Mirage::Layer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(float DeltaTime) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Mirage::Event& e) override;

private:
    Mirage::ShaderLibrary m_ShaderLibrary;

    Mirage::Ref<Mirage::Shader> m_Shader;
    Mirage::Ref<Mirage::VertexArray> m_VertexArray;

    Mirage::Ref<Mirage::Shader> m_SquareShader;
    Mirage::Ref<Mirage::VertexArray> m_SquareVA;

    Mirage::Ref<Mirage::Texture2D> m_Texture;

    Mirage::OrthographicCameraController m_CameraController;

    Mat4 scale = MatScale(Mat4(1.0f), Vec3(0.05f));

    Vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};

