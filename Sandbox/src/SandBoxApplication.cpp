#include <Mirage.h>

#include "ImGui/imgui.h"

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
        
        out vec3 v_Position;
        out vec4 v_Color;
        
        void main()
        {
            v_Position = a_Position;
            v_Color = a_Color;
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
        m_Shader.reset(new Mirage::Shader(vertexShader, fragmentShader));




        
        /*              Square              */
        
        float squareVertices[4 * 3] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
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
        
        out vec3 v_Position;
        out vec4 v_Color;
        
        void main()
        {
            v_Position = a_Position;
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
        }
        )";
        std::string squareFragmentShader = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Position;
        
        void main()
        {
            color = vec4(v_Position + vec3(0.7,0.7,0.7), 1.0);
        }
        )";
        m_SquareShader.reset(new Mirage::Shader(squareVertexShader, squareFragmentShader));
    }

    void OnUpdate(float DeltaTime) override
    {
        MRG_TRACE("Frame time : {0:.3f} ms   -   FPS : {1:.1f}", DeltaTime * 1000.0f, 1.0f / DeltaTime);
        MRG_TRACE("Total elapsed time : {0:.3f} s", Mirage::Application::GetSeconds());
        
        if(Mirage::Input::IsKeyPressed(Mirage::MRG_Key_A) || Mirage::Input::IsKeyPressed(Mirage::MRG_Key_Left))
        {
            m_CameraPosition.x -= (m_CameraMvmtSpeed * DeltaTime);
        }
        else if(Mirage::Input::IsKeyPressed(Mirage::MRG_Key_D) || Mirage::Input::IsKeyPressed(Mirage::MRG_Key_Right))
        {
            m_CameraPosition.x += (m_CameraMvmtSpeed * DeltaTime);
        }
        if(Mirage::Input::IsKeyPressed(Mirage::MRG_Key_W) || Mirage::Input::IsKeyPressed(Mirage::MRG_Key_Up))
        {
            m_CameraPosition.y += (m_CameraMvmtSpeed*DeltaTime);
        }
        else if(Mirage::Input::IsKeyPressed(Mirage::MRG_Key_S) || Mirage::Input::IsKeyPressed(Mirage::MRG_Key_Down))
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
            
        Mirage::Renderer::Submit(m_SquareShader, m_SquareVA);
        Mirage::Renderer::Submit(m_Shader, m_VertexArray);
            
        Mirage::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {
    }

    void OnEvent(Mirage::Event& event) override
    {
        Mirage::EventDispatcher dispatcher(event);
    }
    
private:
    std::shared_ptr<Mirage::Shader> m_Shader;
    std::shared_ptr<Mirage::VertexArray> m_VertexArray;
        
    std::shared_ptr<Mirage::Shader> m_SquareShader;
    std::shared_ptr<Mirage::VertexArray> m_SquareVA;

    Mirage::OrthographicCamera m_Camera;

    Vec3 m_CameraPosition;
    Vec3 m_CameraRotation;
    float m_CameraMvmtSpeed = 5.0f;
    float m_CameraRotSpeed = 50.0f;
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
