#include "MrgPch.h"

#include "Application.h"

#include "Mirage/Renderer/Renderer.h"
namespace Mirage
{
#define Bind_event_FN(x) std::bind(x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;
    
    Application::Application()
        : m_Camera(-1.60, 1.60, -0.90, 0.90, -1.0, 1.0)
    {
        MRG_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallbackFn(Bind_event_FN(&Application::OnEvent));

        m_Window->SetVSync(false);
        
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);


        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };
        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"}
        };
        uint32_t indices[] = {
            0, 1, 2
        };

        m_VertexArray.reset(VertexArray::Create());
        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        vertexBuffer->SetLayout(layout);
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

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
        m_Shader.reset(new Shader(vertexShader, fragmentShader));




        
        /*              Square              */
        
        float squareVertices[4 * 3] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };
        BufferLayout squareLayout = {
            {ShaderDataType::Float3, "a_Position"}
        };
        uint32_t squareIndices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_SquareVA.reset(VertexArray::Create());
        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout(squareLayout);
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        
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
        m_SquareShader.reset(new Shader(squareVertexShader, squareFragmentShader));
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }


    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(Bind_event_FN(&Application::OnWindowClosed));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
            RenderCommand::Clear();

            m_Camera.SetPosition({0.5f, 0.5f,0.0f});
            m_Camera.SetRotation({0.0f,0.0f,180.0f});
            
            Renderer::BeginScene(m_Camera);
            
                Renderer::Submit(m_SquareShader, m_SquareVA);
                Renderer::Submit(m_Shader, m_VertexArray);
            
            Renderer::EndScene();

            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            m_ImGuiLayer->Begin();

            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }

            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClosed(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }
}
