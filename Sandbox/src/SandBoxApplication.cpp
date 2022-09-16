#include <Mirage.h>

#include "glm/gtc/type_ptr.inl"
#include "ImGui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Mirage::Layer
{
public:
    ExampleLayer()
        : Layer("Example"),
          m_Camera(-1.60f, 1.60f, -0.90f, 0.90f, -1.0f, 1.0f),
          m_CameraPosition(0.0f), m_CameraRotation(0.0f)
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

        m_VertexArray.reset(Mirage::VertexArray::Create());
        std::shared_ptr<Mirage::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Mirage::VertexBuffer::Create(vertices, sizeof(vertices)));
        vertexBuffer->SetLayout(layout);
        std::shared_ptr<Mirage::IndexBuffer> indexBuffer;
        indexBuffer.reset(Mirage::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        m_VertexArray->AddVertexBuffer(vertexBuffer);
        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string vertexShader = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;
        
        out vec3 v_Position;
        out vec4 v_Color;
        
        void main()
        {
            v_Position = a_Position;
            v_Color = a_Color;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
        )";
        std::string fragmentShader = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Position;
        in vec4 v_Color;
        
        void main()
        {
            //color = vec4(v_Position + 0.5, 1.0);
            color = v_Color;
        }
        )";
        m_Shader.reset(Mirage::Shader::Create(vertexShader, fragmentShader));




        
        /*              Square              */
        
        float squareVertices[4 * 3] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };
        Mirage::BufferLayout squareLayout = {
            {Mirage::ShaderDataType::Float3, "a_Position"}
        };
        uint32_t squareIndices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_SquareVA.reset(Mirage::VertexArray::Create());
        std::shared_ptr<Mirage::VertexBuffer> squareVB;
        squareVB.reset(Mirage::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout(squareLayout);
        std::shared_ptr<Mirage::IndexBuffer> squareIB;
        squareIB.reset(Mirage::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        
        m_SquareVA->AddVertexBuffer(squareVB);
        m_SquareVA->SetIndexBuffer(squareIB);
        
        std::string squareVertexShader = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;
        
        
        out vec3 v_Position;
        
        void main()
        {
            v_Position = a_Position;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
        )";
        std::string squareFragmentShader = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Position;

        uniform vec4 u_Color;
        
        void main()
        {
            color = u_Color;
        }
        )";
        m_SquareShader.reset(Mirage::Shader::Create(squareVertexShader, squareFragmentShader));
    }

    void OnUpdate(float DeltaTime) override
    {
        // MRG_TRACE("Frame time : {0:.3f} ms   -   FPS : {1:.1f}", DeltaTime * 1000.0f, 1.0f / DeltaTime);
        // MRG_TRACE("Total elapsed time : {0:.3f} s", Mirage::Application::GetSeconds());

        if (Mirage::Input::IsKeyPressed(Mirage::MRG_Key_A))
        {
            m_CameraPosition.x -= (m_CameraMvmtSpeed * DeltaTime);
        }
        else if (Mirage::Input::IsKeyPressed(Mirage::MRG_Key_D))
        {
            m_CameraPosition.x += (m_CameraMvmtSpeed * DeltaTime);
        }
        if (Mirage::Input::IsKeyPressed(Mirage::MRG_Key_W))
        {
            m_CameraPosition.y += (m_CameraMvmtSpeed * DeltaTime);
        }
        else if (Mirage::Input::IsKeyPressed(Mirage::MRG_Key_S))
        {
            m_CameraPosition.y -= (m_CameraMvmtSpeed * DeltaTime);
        }

        if (Mirage::Input::IsKeyPressed(Mirage::MRG_Key_Q))
        {
            m_CameraRotation.z += (m_CameraRotSpeed * DeltaTime);
        }
        else if (Mirage::Input::IsKeyPressed(Mirage::MRG_Key_E))
        {
            m_CameraRotation.z -= (m_CameraRotSpeed * DeltaTime);
        }

        Mirage::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
        Mirage::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);
                
        Mirage::Renderer::BeginScene(m_Camera);
        
        std::dynamic_pointer_cast<Mirage::OpenGLShader>(m_SquareShader)->Bind();
        std::dynamic_pointer_cast<Mirage::OpenGLShader>(m_SquareShader)->UploadUniformFloat4("u_Color", m_SquareColor);
        
        for(int i = 0; i < 25; ++i)
        {
            for(int j = 0; j < 25; ++j)
            {
                Vec3 Pos = Vec3(0.06f * i , 0.06f * j, 0.0f);
                Mat4 SquareTransform = MatTranslate(Mat4(1.0f), Pos) * scale;
                Mirage::Renderer::Submit(m_SquareShader, m_SquareVA, SquareTransform);
            }
        }
        Mirage::Renderer::Submit(m_Shader, m_VertexArray);
            
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
        Mirage::EventDispatcher dispatcher(event);
    }
    
private:
    Mirage::Ref<Mirage::Shader> m_Shader;
    Mirage::Ref<Mirage::VertexArray> m_VertexArray;
    
    Mirage::Ref<Mirage::Shader> m_SquareShader;
    Mirage::Ref<Mirage::VertexArray> m_SquareVA;

    Mirage::OrthographicCamera m_Camera;

    Vec3 m_CameraPosition;
    Vec3 m_CameraRotation;
    float m_CameraMvmtSpeed = 5.0f;
    float m_CameraRotSpeed = 50.0f;
    
    Mat4 scale = MatScale(Mat4(1.0f), Vec3(0.05f));

    Vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};

class Sandbox : public Mirage::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {
    }
};

Mirage::Application* Mirage::CreateApplication()
{
    return new Sandbox();
}
