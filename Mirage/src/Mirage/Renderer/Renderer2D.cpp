#include "MrgPch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Mirage
{
    namespace Renderer2D
    {
        struct Storage
        {
            Ref<VertexArray> QuadVertexArray;
            Ref<Shader> Shader;
        };

        static Storage* s_Data;

        void Init()
        {
            s_Data = new Storage();

            float squareVertices[4 * 5] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
            };
            BufferLayout squareLayout = {
                {ShaderDataType::Float3, "a_Position"},
                {Mirage::ShaderDataType::Float2, "a_TexCoord"}
            };
            uint32_t squareIndices[] = {
                0, 1, 2,
                2, 3, 0
            };

            s_Data->QuadVertexArray = VertexArray::Create();
            Ref<Mirage::VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
            squareVB->SetLayout(squareLayout);
            Ref<Mirage::IndexBuffer> squareIB = IndexBuffer::Create(squareIndices,
                                                                    sizeof(squareIndices) / sizeof(uint32_t));

            s_Data->QuadVertexArray->AddVertexBuffer(squareVB);
            s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

            s_Data->Shader = Shader::Create("assets/shaders/Standard.glsl");
        }

        void Shutdown()
        {
            delete s_Data;
        }

        void BeginScene(const OrthographicCamera& camera)
        {
            s_Data->Shader->Bind();
            s_Data->Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        }

        void EndScene()
        {
        }


        namespace Draw
        {
            void Quad(Primitives::Quad quad)
            {
                s_Data->Shader->Bind();
                s_Data->Shader->SetFloat4("u_Color", quad.color);

                Mat4 transform = MatTranslate(Mat4(1.0f), quad.position) *
                    MatRotate(Mat4(1.0f), quad.rotation.x, {1.0f, 0.0f, 0.0f}) *
                    MatRotate(Mat4(1.0f), quad.rotation.y, {0.0f, 1.0f, 0.0f}) *
                    MatRotate(Mat4(1.0f), quad.rotation.z, {0.0f, 0.0f, 1.0f}) *
                    MatScale(Mat4(1.0f), quad.scale);
                s_Data->Shader->SetMat4("u_Transform", transform);

                quad.texture->Bind();

                s_Data->Shader->SetInt("u_Texture", 0);
                s_Data->Shader->SetFloat2("u_Tiling", quad.tiling);
                s_Data->Shader->SetFloat2("u_Offset", quad.offset);

                s_Data->QuadVertexArray->Bind();
                RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
            }
        }
    }
}
