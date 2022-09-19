#include <Mirage.h>

#include "Sandbox2D.h"
#include "glm/gtc/type_ptr.inl"
#include "ImGui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
/* *******Entry point******* */
#include "Mirage/Core/EntryPoint.h"

class ExampleLayer : public Mirage::Layer
{
public:
    ExampleLayer()
        : Layer("Example"),
          m_CameraController(1280.0f / 720.0f, true)
    {
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };
        Mirage::BufferLayout layout = {
            {Mirage::ShaderDataType::Float3, "a_Position"},
            {Mirage::ShaderDataType::Float4, "a_Color"}
        };
        uint32_t indices[] = {
            0, 1, 2
        };

        m_VertexArray = Mirage::VertexArray::Create();
        Mirage::Ref<Mirage::VertexBuffer> vertexBuffer = Mirage::VertexBuffer::Create(vertices, sizeof(vertices));
        vertexBuffer->SetLayout(layout);
        Mirage::Ref<Mirage::IndexBuffer> indexBuffer = Mirage::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

        m_VertexArray->AddVertexBuffer(vertexBuffer);
        m_VertexArray->SetIndexBuffer(indexBuffer);
        m_Shader = Mirage::Shader::Create("assets/shaders/Triangle.glsl");


        /*              Square              */

        float squareVertices[4 * 5] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };
        Mirage::BufferLayout squareLayout = {
            {Mirage::ShaderDataType::Float3, "a_Position"},
            {Mirage::ShaderDataType::Float2, "a_TexCoord"}
        };
        uint32_t squareIndices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_SquareVA = Mirage::VertexArray::Create();
        Mirage::Ref<Mirage::VertexBuffer> squareVB = Mirage::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
        squareVB->SetLayout(squareLayout);
        Mirage::Ref<Mirage::IndexBuffer> squareIB = Mirage::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

        m_SquareVA->AddVertexBuffer(squareVB);
        m_SquareVA->SetIndexBuffer(squareIB);

        m_SquareShader = Mirage::Shader::Create("assets/shaders/FlatColor.glsl");
        m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

        m_Texture = Mirage::Texture2D::Create("assets/textures/CheckerBoard.png");

        std::dynamic_pointer_cast<Mirage::OpenGLShader>(m_ShaderLibrary.Get("Texture"))->Bind();
        std::dynamic_pointer_cast<Mirage::OpenGLShader>(m_ShaderLibrary.Get("Texture"))->UploadUniformInt(
            "u_Texture", 0);
    }

    void OnUpdate(float DeltaTime) override
    {
        // MRG_TRACE("Frame time : {0:.3f} ms   -   FPS : {1:.1f}", DeltaTime * 1000.0f, 1.0f / DeltaTime);
        // MRG_TRACE("Total elapsed time : {0:.3f} s", Mirage::Application::GetSeconds());

        // Update
        m_CameraController.OnUpdate(DeltaTime);


        // Render
        Mirage::RenderCommand::SetClearColor({0.15f, 0.15f, 0.15f, 1.0f});
        Mirage::RenderCommand::Clear();

        Mirage::Renderer::BeginScene(m_CameraController.GetCamera());

        std::dynamic_pointer_cast<Mirage::OpenGLShader>(m_SquareShader)->Bind();
        std::dynamic_pointer_cast<Mirage::OpenGLShader>(m_SquareShader)->UploadUniformFloat4("u_Color", m_SquareColor);

        for (int i = 0; i < 25; ++i)
        {
            for (int j = 0; j < 25; ++j)
            {
                Vec3 Pos = Vec3(0.06f * i, 0.06f * j, 0.0f);
                Mat4 SquareTransform = MatTranslate(Mat4(1.0f), Pos) * scale;
                Mirage::Renderer::Submit(m_SquareShader, m_SquareVA, SquareTransform);
            }
        }
        m_Texture->Bind();
        Mirage::Renderer::Submit(m_ShaderLibrary.Get("Texture"), m_SquareVA, MatScale(Mat4(1.0f), Vec3(1.5f)));
        // Mirage::Renderer::Submit(m_FlatColorShader, m_VertexArray);

        Mirage::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    void OnEvent(Mirage::Event& event) override
    {
        m_CameraController.OnEvent(event);
    }

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

class Sandbox : public Mirage::Application
{
public:
    Sandbox()
    {
    //    PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {
    }
};

Mirage::Application* Mirage::CreateApplication()
{
    return new Sandbox();
}
