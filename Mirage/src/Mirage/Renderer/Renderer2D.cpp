#include "MrgPch.h"

#include "Mirage/Renderer/Renderer2D.h"

#include "Mirage/Renderer/RenderCommand.h"
#include "Mirage/Renderer/Shader.h"
#include "Mirage/Renderer/VertexArray.h"

namespace Mirage
{
    namespace Renderer2D
    {
        struct Storage
        {
            Ref<VertexArray> QuadVertexArray;
            Ref<Shader> Shader;
            Ref<Texture2D> WhiteTexture;
        };

        static Storage* s_Data;

        void Init()
        {
            MRG_PROFILE_FUNCTION();

            s_Data = new Storage();

            float squareVertices[4 * 5] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
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

            s_Data->WhiteTexture = Texture2D::Create(1, 1);
            uint32_t whiteTextureData = 0xffffffff;
            s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

            s_Data->QuadVertexArray->AddVertexBuffer(squareVB);
            s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

            s_Data->Shader = Shader::Create("assets/shaders/Standard.glsl");
        }

        void Shutdown()
        {
            MRG_PROFILE_FUNCTION();
        
            delete s_Data;
        }

        void BeginScene(const OrthographicCamera& camera)
        {
            MRG_PROFILE_FUNCTION();
        
            s_Data->Shader->Bind();
            s_Data->Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        }

        void EndScene()
        {
            MRG_PROFILE_FUNCTION();
        
        }


        namespace Draw
        {
            void Quad(Primitives::Quad quad)
            {
                MRG_PROFILE_FUNCTION();
        
                s_Data->Shader->SetFloat4("u_Color", quad.color);
            
                Mat4 transform ;
                {
                    MRG_PROFILE_SCOPE("calculating transform");
                    transform = MatTranslate(Mat4(1.0f), quad.position) *
                       MatRotate(Mat4(1.0f), quad.rotation.x, {1.0f, 0.0f, 0.0f}) *
                       MatRotate(Mat4(1.0f), quad.rotation.y, {0.0f, 1.0f, 0.0f}) *
                       MatRotate(Mat4(1.0f), quad.rotation.z, {0.0f, 0.0f, 1.0f}) *
                       MatScale(Mat4(1.0f), quad.scale);
                }
                s_Data->Shader->SetMat4("u_Transform", transform);
                if (quad.texture)
                {
                    quad.texture->Bind();
                }
                else
                {
                    s_Data->WhiteTexture->Bind();
                }
                s_Data->Shader->SetInt("u_Texture", 0);
                s_Data->Shader->SetFloat2("u_Tiling", quad.tiling);
                s_Data->Shader->SetFloat2("u_Offset", quad.offset);
            
                s_Data->QuadVertexArray->Bind();
                RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
            }
        }
    }
}
