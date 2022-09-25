#include "MrgPch.h"

#include "Mirage/Renderer/Renderer2D.h"

#include "Mirage/Renderer/RenderCommand.h"
#include "Mirage/Renderer/Shader.h"
#include "Mirage/Renderer/VertexArray.h"

namespace Mirage
{
    namespace Renderer2D
    {

        struct QuadVertex
        {
            Vec3 Position;
            Vec4 Color;
            Vec2 TexCoord;
            float TexIndex;
            Vec2 Tiling;
            Vec2 Offset;
        };
        
        struct Renderer2DData
        {
            const uint32_t MaxQuads = 100000;
            const uint32_t MaxVertices = MaxQuads * 4;
            const uint32_t MaxIndices = MaxQuads * 6;
            static const uint32_t MaxTextureSlots = 32;        //TODO : RenderCaps
            
            Ref<VertexArray> QuadVertexArray;
            Ref<VertexBuffer> QuadVertexBuffer;
            Ref<Shader> Shader;
            Ref<Texture2D> WhiteTexture;

            uint32_t QuadIndexCount = 0;

            QuadVertex* QuadVertexBufferBase = nullptr;
            QuadVertex* QuadVertexBufferPtr = nullptr;

            std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
            uint32_t TextureSlotIndex = 1;

            Vec4 QuadVertexPositions[4];
        };
        
        static Renderer2DData s_Data;

        void Init()
        {
            MRG_PROFILE_FUNCTION();
            
            s_Data.QuadVertexArray = VertexArray::Create();
            s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
            
            BufferLayout squareLayout = {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float4, "a_Color"},
                {ShaderDataType::Float2, "a_TexCoord"},
                {ShaderDataType::Float1, "a_TexIndex"},
                {ShaderDataType::Float2, "a_Tiling"},
                {ShaderDataType::Float2, "a_Offset"},
            };
            s_Data.QuadVertexBuffer->SetLayout(squareLayout);
            s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

            s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
            
            uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

            uint32_t offset = 0;
            for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
            {
                quadIndices[i + 0] = offset + 0;
                quadIndices[i + 1] = offset + 1;
                quadIndices[i + 2] = offset + 2;
                                     
                quadIndices[i + 3] = offset + 2;
                quadIndices[i + 4] = offset + 3;
                quadIndices[i + 5] = offset + 0;

                offset += 4;
            }
            
            Ref<Mirage::IndexBuffer> QuadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
            s_Data.QuadVertexArray->SetIndexBuffer(QuadIB);

            delete[] quadIndices;

            s_Data.WhiteTexture = Texture2D::Create(1, 1);
            uint32_t whiteTextureData = 0xffffffff;
            s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

            int32_t samplers[s_Data.MaxTextureSlots];
            for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
                samplers[i] = i;
            
            s_Data.Shader = Shader::Create("assets/shaders/Standard.glsl");
            s_Data.Shader->Bind();
            s_Data.Shader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

            s_Data.TextureSlots[0] = s_Data.WhiteTexture;

            s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
            s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f};
            s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f};
            s_Data.QuadVertexPositions[3] = {-0.5f,  0.5f, 0.0f, 1.0f};
        }

        void Shutdown()
        {
            MRG_PROFILE_FUNCTION();
        }

        void BeginScene(const OrthographicCamera& camera)
        {
            MRG_PROFILE_FUNCTION();
        
            s_Data.Shader->Bind();
            s_Data.Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

            s_Data.QuadIndexCount = 0;
            s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        }

        void EndScene()
        {
            MRG_PROFILE_FUNCTION();

            uint32_t size = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
            
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, size);

            Flush();
        }

        void Flush()
        {
            //Bind textures
            for(uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            {
                s_Data.TextureSlots[i]->Bind(i);
            }
            
            RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        }

        namespace Draw
        {
            
            void Quad(const Primitives::Quad& quad, const Ref<Texture2D>& texture, const Vec2& tiling, const Vec2& offset)
            {
                MRG_PROFILE_FUNCTION();

                constexpr Vec4 color = {1.0f, 0.0f, 1.0f, 1.0f};
                float TextureIndex = 0.0f;

                if(texture)
                {
                    for(uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
                    {
                        if (*(s_Data.TextureSlots[i].get()) == *(texture.get()))
                        {
                            TextureIndex = (float)i;
                            break;
                        }
                    }
                
                    if (TextureIndex == 0.0f)
                    {
                        TextureIndex = (float)s_Data.TextureSlotIndex;
                        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
                        s_Data.TextureSlotIndex++;
                    }
                }

                Mat4 transform;
                if(quad.rotation.x || quad.rotation.y)
                {
                    transform = MatTranslate(Mat4(1.0f), quad.position) *
                                     MatRotate(Mat4(1.0f), Radians(quad.rotation.x), {1.0f, 0.0f, 0.0f}) *
                                     MatRotate(Mat4(1.0f), Radians(quad.rotation.y), {0.0f, 1.0f, 0.0f}) *
                                     MatRotate(Mat4(1.0f), Radians(quad.rotation.z), {0.0f, 0.0f, 1.0f}) *
                                     MatScale(Mat4(1.0f), quad.scale);
                }
                else
                {
                    transform = MatTranslate(Mat4(1.0f), quad.position) *
                                     MatRotate(Mat4(1.0f), Radians(quad.rotation.z), {0.0f, 0.0f, 1.0f}) *
                                     MatScale(Mat4(1.0f), quad.scale);
                }
                
                s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
                s_Data.QuadVertexBufferPtr->Color = quad.color;
                s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
                s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
                s_Data.QuadVertexBufferPtr->Tiling = tiling;
                s_Data.QuadVertexBufferPtr->Offset = offset;
                s_Data.QuadVertexBufferPtr++;

                s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
                s_Data.QuadVertexBufferPtr->Color = quad.color;
                s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 0.0f};
                s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
                s_Data.QuadVertexBufferPtr->Tiling = tiling;
                s_Data.QuadVertexBufferPtr->Offset = offset;
                s_Data.QuadVertexBufferPtr++;
                
                s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
                s_Data.QuadVertexBufferPtr->Color = quad.color;
                s_Data.QuadVertexBufferPtr->TexCoord = {1.0f, 1.0f};
                s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
                s_Data.QuadVertexBufferPtr->Tiling = tiling;
                s_Data.QuadVertexBufferPtr->Offset = offset;
                s_Data.QuadVertexBufferPtr++;
                
                s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
                s_Data.QuadVertexBufferPtr->Color = quad.color;
                s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 1.0f};
                s_Data.QuadVertexBufferPtr->TexIndex = TextureIndex;
                s_Data.QuadVertexBufferPtr->Tiling = tiling;
                s_Data.QuadVertexBufferPtr->Offset = offset;
                s_Data.QuadVertexBufferPtr++;

                s_Data.QuadIndexCount += 6;
                            
                /*Mat4 transform ;
                {
                    MRG_PROFILE_SCOPE("calculating transform");
                    transform = MatTranslate(Mat4(1.0f), quad.position) *
                       MatRotate(Mat4(1.0f), quad.rotation.x, {1.0f, 0.0f, 0.0f}) *
                       MatRotate(Mat4(1.0f), quad.rotation.y, {0.0f, 1.0f, 0.0f}) *
                       MatRotate(Mat4(1.0f), quad.rotation.z, {0.0f, 0.0f, 1.0f}) *
                       MatScale(Mat4(1.0f), quad.scale);
                }
                s_Data.Shader->SetMat4("u_Transform", transform);
                if (texture)
                {
                    texture->Bind();
                }
                else
                {
                    s_Data.WhiteTexture->Bind();
                }
                s_Data.Shader->SetInt("u_Texture", 0);
                s_Data.Shader->SetFloat2("u_Tiling", tiling);
                s_Data.Shader->SetFloat2("u_Offset", offset);
                      
                s_Data.QuadVertexArray->Bind();
                RenderCommand::DrawIndexed(s_Data.QuadVertexArray);*/
            }
        }
    }
}
