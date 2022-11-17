#include "MrgPch.h"

#include "Mirage/Renderer/Renderer2D.h"

#include "Mirage/Renderer/RenderCommand.h"
#include "Mirage/Renderer/Shader.h"
#include "Mirage/Renderer/UniformBuffer.h"
#include "Mirage/Renderer/VertexArray.h"

namespace Mirage
{
    namespace Renderer2D
    {

        struct QuadVertex
        {
            Vec3 Position;
            Vec4 Color;
            Vec2 TextureCoord;
            float TextureIndex;
            Vec2 Tiling;
            Vec2 Offset;

            // Editor only !!!
            int EntityID = -1;
        };
        
        struct Renderer2DData
        {
            static const uint32_t MaxQuads = 500;
            static const uint32_t MaxVertices = MaxQuads * 4;
            static const uint32_t MaxIndices = MaxQuads * 6;
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

        	Stats Stats;
        	
        	struct CameraData
        	{
        		glm::mat4 ViewProjection;
        	};
        	CameraData CameraBuffer;
        	Ref<UniformBuffer> CameraUniformBuffer;
        };
        
        static Renderer2DData s_Data;

        void Init()
        {
            MRG_PROFILE_FUNCTION();
            
            s_Data.QuadVertexArray = VertexArray::Create();
            s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
            
            BufferLayout squareLayout = {
                {ShaderDataType::Float3, "a_Position"   },
                {ShaderDataType::Float4, "a_Color"      },
                {ShaderDataType::Float2, "a_TexCoord"   },
                {ShaderDataType::Float1, "a_TexIndex"   },
                {ShaderDataType::Float2, "a_Offset"     },
                {ShaderDataType::Float2, "a_Tiling"     },
                {ShaderDataType::Int1,   "a_EntityID"   }
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
        	
            s_Data.TextureSlots[0] = s_Data.WhiteTexture;

            s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
            s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f};
            s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f};
            s_Data.QuadVertexPositions[3] = {-0.5f,  0.5f, 0.0f, 1.0f};
            
        	s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
        }

        void Shutdown()
        {
            MRG_PROFILE_FUNCTION();
            
            delete[] s_Data.QuadVertexBufferBase;
        }

        void BeginScene(const Camera& camera, const Mat4& transform)
        {
            MRG_PROFILE_FUNCTION();
            
            s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * Inverse(transform);
            s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

            StartBatch();
        }
        void BeginScene(const OrthographicCamera& camera)
        {
        	MRG_PROFILE_FUNCTION();
        	
        	s_Data.Shader->Bind();
        	s_Data.Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        	
			StartBatch();
        }
        void BeginScene(const EditorCamera& camera)
        {
        	MRG_PROFILE_FUNCTION();
        	
        	s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
        	s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

            StartBatch();
        }

        void EndScene()
        {
            Flush();
        }
        
        void StartBatch()
        {
            s_Data.QuadIndexCount = 0;
            s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

            s_Data.TextureSlotIndex = 1;
        }

        void Flush()
        {
            if (s_Data.QuadIndexCount == 0)
                return;

            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
            
            //Bind textures
            for(uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            {
                s_Data.TextureSlots[i]->Bind(i);
            }
            
            s_Data.Shader->Bind();
            RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
            
            s_Data.Stats.DrawCalls++;
        }

        void NextBatch()
        {
            EndScene();
            
            s_Data.QuadIndexCount = 0;
            s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
            
            s_Data.TextureSlotIndex = 1;
        }
        
        namespace Draw
        {
            void Quad(const Mat4& transform, const Vec4& color, const Ref<Texture2D> texture, const Vec2& tiling, const Vec2& offset, int entityID)
            {
                Primitives::Quad quad { transform, color, texture, tiling, offset };

                constexpr size_t quadVertexCount = 4;
                constexpr Vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

                if(s_Data.QuadIndexCount >= s_Data.MaxIndices)
                {
                    NextBatch();
                }

                float TextureIndex = 0.0f;
                if(quad.texture)
                {
                    for(uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
                    {
                        if (*(s_Data.TextureSlots[i]) == *(quad.texture))
                        {
                            TextureIndex = (float)i;
                            break;
                        }
                    }
                
                    if (TextureIndex == 0.0f)
                    {
                        if(s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
                        {
                    NextBatch();
                        }
                        
                        TextureIndex = (float)s_Data.TextureSlotIndex;
                        s_Data.TextureSlots[s_Data.TextureSlotIndex] = quad.texture;
                        s_Data.TextureSlotIndex++;
                    }
                }
                
                for (size_t i = 0; i < quadVertexCount; i++)
                {
                    s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
                    s_Data.QuadVertexBufferPtr->Color = quad.color;
                    s_Data.QuadVertexBufferPtr->TextureCoord = textureCoords[i];
                    s_Data.QuadVertexBufferPtr->TextureIndex = TextureIndex;
                    s_Data.QuadVertexBufferPtr->Tiling = quad.Tiling;
                    s_Data.QuadVertexBufferPtr->Offset = quad.Offset;
                    s_Data.QuadVertexBufferPtr->EntityID = entityID;
                    s_Data.QuadVertexBufferPtr++;
                }

                s_Data.QuadIndexCount += 6;

                s_Data.Stats.QuadCount++;
            }
            
            void Quad(const Primitives::Quad& quad, int entityID)
            {
                Draw::Quad(quad.transform, quad.color, quad.texture, quad.Tiling, quad.Offset, entityID);
            }
            
            void Sprite(const Mat4& transform, SpriteRendererComponent& sprite, int entityID)
            {
                Draw::Quad(transform, sprite.Color, sprite.Texture, sprite.Tiling, sprite.Offset, entityID);
            }
        }

        Stats GetStats()
        {
            return s_Data.Stats;
        }

        void ResetStats()
        {
            memset(&s_Data.Stats, 0, sizeof(Stats));
        }

    }
}
