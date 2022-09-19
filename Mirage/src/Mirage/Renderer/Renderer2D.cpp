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
            Ref<Shader> FlatColorShader;
        };

        static Storage* s_Data;

        void Init()
        {
            s_Data = new Storage();

            float squareVertices[4 * 5] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.5f, 0.5f, 0.0f,
                -0.5f, 0.5f, 0.0f
            };
            BufferLayout squareLayout = {
                {ShaderDataType::Float3, "a_Position"}
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

            s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
        }

        void Shutdown()
        {
            delete s_Data;
        }

        void BeginScene(const OrthographicCamera& camera)
        {
            s_Data->FlatColorShader->Bind();
            s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        }

        void EndScene()
        {
        }


        namespace Draw
        {
            void Quad(const Vec2& position, float rotation, const Vec2& scale, const Vec4& color)
            {
                Quad({position.x, position.y, 0.0f}, {0.0f, 0.0f, rotation}, {scale.x, scale.y, 0.0f}, color);
            }

            void Quad(const Vec3& position, const Vec3& rotation, const Vec3& scale, const Vec4& color)
            {
                s_Data->FlatColorShader->Bind();
                s_Data->FlatColorShader->SetFloat4("u_Color", color);

                Mat4 transform = MatTranslate(Mat4(1.0f), position) *
                    MatRotate(Mat4(1.0f), rotation.x, {1.0f, 0.0f, 0.0f}) *
                    MatRotate(Mat4(1.0f), rotation.y, {0.0f, 1.0f, 0.0f}) *
                    MatRotate(Mat4(1.0f), rotation.z, {0.0f, 0.0f, 1.0f}) *
                    MatScale(Mat4(1.0f), scale);

                s_Data->FlatColorShader->SetMat4("u_Transform", transform);

                s_Data->QuadVertexArray->Bind();
                RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
            }
        }
    }
}
