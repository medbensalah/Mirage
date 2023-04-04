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

        struct CircleVertex
        {
            Vec3 WorldPosition;
            Vec3 LocalPosition;
        	float Thickness;
        	float Fade;
            Vec4 Color;
            Vec2 TextureCoord;
            float TextureIndex;
            Vec2 Tiling;
            Vec2 Offset;

            // Editor only !!!
            int EntityID = -1;
        };
        
    	struct LineVertex
    	{
    		Vec3 Position;
    		Vec4 Color;

    		// Editor only !!!
    		int EntityID = -1;
    	};
    	
        struct Renderer2DData
        {
            static const uint32_t MaxQuads = 500;
            static const uint32_t MaxQuadVertices = MaxQuads * 4;
            static const uint32_t MaxQuadIndices = MaxQuads * 6;
            static const uint32_t MaxCircles = 500;
            static const uint32_t MaxCircleVertices = MaxQuads * 4;
            static const uint32_t MaxCircleIndices = MaxQuads * 6;
            static const uint32_t MaxTextureSlots = 32;        //TODO : RenderCaps
            
            Ref<VertexArray> QuadVertexArray;
            Ref<VertexBuffer> QuadVertexBuffer;
            Ref<Shader> QuadShader;
            Ref<Texture2D> WhiteTexture;
        	
        	Ref<VertexArray> CircleVertexArray;
        	Ref<VertexBuffer> CircleVertexBuffer;
        	Ref<Shader> CircleShader;
        	
        	Ref<VertexArray> LineVertexArray;
        	Ref<VertexBuffer> LineVertexBuffer;
        	Ref<Shader> LineShader;


        	

            uint32_t QuadIndexCount = 0;
            QuadVertex* QuadVertexBufferBase = nullptr;
            QuadVertex* QuadVertexBufferPtr = nullptr;

            uint32_t CircleIndexCount = 0;
            CircleVertex* CircleVertexBufferBase = nullptr;
            CircleVertex* CircleVertexBufferPtr = nullptr;

            uint32_t LineVertexCount = 0;
            LineVertex* LineVertexBufferBase = nullptr;
            LineVertex* LineVertexBufferPtr = nullptr;
        	float LineWidth = 2.0f;

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
            s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadVertices * sizeof(QuadVertex));
            
            BufferLayout squareLayout = {
                {ShaderDataType::Float3, "a_Position"   },
                {ShaderDataType::Float4, "a_Color"      },
                {ShaderDataType::Float2, "a_TexCoord"   },
                {ShaderDataType::Float1, "a_TexIndex"   },
                {ShaderDataType::Float2, "a_Tiling"     },
                {ShaderDataType::Float2, "a_Offset"     },
                {ShaderDataType::Int1,   "a_EntityID"   }
            };
            s_Data.QuadVertexBuffer->SetLayout(squareLayout);
            s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

            s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxQuadVertices];
            
            uint32_t* quadIndices = new uint32_t[s_Data.MaxQuadIndices];

            uint32_t offset = 0;
            for (uint32_t i = 0; i < s_Data.MaxQuadIndices; i += 6)
            {
                quadIndices[i + 0] = offset + 0;
                quadIndices[i + 1] = offset + 1;
                quadIndices[i + 2] = offset + 2;
                                     
                quadIndices[i + 3] = offset + 2;
                quadIndices[i + 4] = offset + 3;
                quadIndices[i + 5] = offset + 0;

                offset += 4;
            }
            
            Ref<Mirage::IndexBuffer> QuadIB = IndexBuffer::Create(quadIndices, s_Data.MaxQuadIndices);
            s_Data.QuadVertexArray->SetIndexBuffer(QuadIB);

            delete[] quadIndices;

			// ------------------------------ Circle ------------------------------

        	s_Data.CircleVertexArray = VertexArray::Create();
        	s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadVertices * sizeof(CircleVertex));
            
        	BufferLayout circleLayout = {
        		{ShaderDataType::Float3, "a_WorldPosition"	},
        		{ShaderDataType::Float3, "a_LocalPosition"	},
				{ShaderDataType::Float1, "a_Thickness"		},
				{ShaderDataType::Float1, "a_Fade"			},
				{ShaderDataType::Float4, "a_Color"      	},
				{ShaderDataType::Float2, "a_TexCoord"   	},
				{ShaderDataType::Float1, "a_TexIndex"   	},
				{ShaderDataType::Float2, "a_Tiling"     	},
				{ShaderDataType::Float2, "a_Offset"     	},
				{ShaderDataType::Int1,   "a_EntityID"   	}
        	};
        	s_Data.CircleVertexBuffer->SetLayout(circleLayout);
        	s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
            s_Data.CircleVertexArray->SetIndexBuffer(QuadIB);		// possible to use quad IB
        	s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxQuadVertices];

			// ------------------------------ Line ------------------------------

        	s_Data.LineVertexArray = VertexArray::Create();
        	s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadVertices * sizeof(LineVertex));
            
        	BufferLayout LineLayout = {
        		{ShaderDataType::Float3, "a_Position"	},
				{ShaderDataType::Float4, "a_Color"      	},
				{ShaderDataType::Int1,   "a_EntityID"   	}
        	};
        	s_Data.LineVertexBuffer->SetLayout(LineLayout);
        	s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
        	s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxQuadVertices];

        	

            s_Data.WhiteTexture = Texture2D::Create(1, 1);
            uint32_t whiteTextureData = 0xffffffff;
            s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

            int32_t samplers[s_Data.MaxTextureSlots];
            for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
                samplers[i] = i;
            
            s_Data.QuadShader = Shader::Create("assets/shaders/DefaultQuad.glsl");
            s_Data.CircleShader = Shader::Create("assets/shaders/DefaultCircle.glsl");
            s_Data.LineShader = Shader::Create("assets/shaders/DefaultLine.glsl");
        	
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

        	s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
        	s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
        	
        	StartBatch();
        }
        void BeginScene(const EditorCamera& camera)
        {
        	MRG_PROFILE_FUNCTION();
        	
        	s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
        	s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

        	StartBatch();
        }

    	void StartBatch()
        {
        	StartQuadBatch();
        	StartCircleBatch();
        	StartLineBatch();

        	s_Data.TextureSlotIndex = 1;
        }
    	void StartQuadBatch()
        {
        	s_Data.QuadIndexCount = 0;
        	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        }
    	void StartCircleBatch()
        {
        	s_Data.CircleIndexCount = 0;
        	s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
        }
    	void StartLineBatch()
        {
        	s_Data.LineVertexCount = 0;
        	s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
        }
    	
        void EndScene()
        {
        	//Bind textures
        	for(uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        	{
        		s_Data.TextureSlots[i]->Bind(i);
        	}
        	
            FlushQuad();
            FlushCircle();
            FlushLine();
        }        

        void FlushQuad()
        {
            if (s_Data.QuadIndexCount == 0)
                return;

            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
            
            s_Data.QuadShader->Bind();
            RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
            
            s_Data.Stats.DrawCalls++;
        }
        void FlushCircle()
        {
            if (s_Data.CircleIndexCount == 0)
                return;

            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
            s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);
            
            s_Data.CircleShader->Bind();
            RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
            
            s_Data.Stats.DrawCalls++;
        }
        void FlushLine()
        {
            if (s_Data.LineVertexCount == 0)
                return;

            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
            s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);
            
            s_Data.LineShader->Bind();
        	RenderCommand::SetLineWidth(s_Data.LineWidth);
            RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
            
            s_Data.Stats.DrawCalls++;
        }

        void NextQuadBatch()
        {
            FlushQuad();
            
            s_Data.QuadIndexCount = 0;
            s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
            
            s_Data.TextureSlotIndex = 1;
        }
        void NextCircleBatch()
        {
            FlushCircle();
            
            s_Data.CircleIndexCount = 0;
            s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
            
            s_Data.TextureSlotIndex = 1;
        }
        void NextLineBatch()
        {
            FlushLine();
            
            s_Data.LineVertexCount = 0;
            s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
            
            s_Data.TextureSlotIndex = 1;
        }
        
        namespace Draw
        {
            void Quad(const Mat4& transform, const Vec4& color, const Ref<Texture2D> texture, const Vec2& tiling, const Vec2& offset, int entityID)
            {
                Primitives::Quad quad { transform, color, texture, tiling, offset };

                constexpr size_t quadVertexCount = 4;
                constexpr Vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

                if(s_Data.QuadIndexCount >= s_Data.MaxQuadIndices)
                {
                    NextQuadBatch();
                }

                float TextureIndex = 0.0f;
                if (quad.texture)
                {
	                for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
	                {
		                if (*(s_Data.TextureSlots[i]) == *(quad.texture))
		                {
			                TextureIndex = (float)i;
			                break;
		                }
	                }

	                if (TextureIndex == 0.0f)
	                {
		                if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
		                {
			                NextQuadBatch();
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

            void Circle(const glm::mat4& transform, CircleRendererComponent& circle, int entityID)
            {
	            Primitives::Circle circlep{
		            transform, circle.Thickness, circle.Fade, circle.Color, circle.Texture, circle.Tiling, circle.Offset
	            };
	            constexpr size_t circleVertexCount = 4;
	            constexpr Vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

	            // TODO: make flush for circles
	            if (s_Data.CircleIndexCount >= s_Data.MaxCircleIndices)
	            {
		            NextCircleBatch();
	            }

	            float TextureIndex = 0.0f;
	            if (circlep.texture)
	            {
		            for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		            {
			            if (*(s_Data.TextureSlots[i]) == *(circlep.texture))
			            {
				            TextureIndex = (float)i;
				            break;
			            }
		            }

		            if (TextureIndex == 0.0f)
		            {
			            if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			            {
				            NextCircleBatch();
			            }

			            TextureIndex = (float)s_Data.TextureSlotIndex;
			            s_Data.TextureSlots[s_Data.TextureSlotIndex] = circlep.texture;
			            s_Data.TextureSlotIndex++;
		            }
	            }

	            for (size_t i = 0; i < circleVertexCount; i++)
	            {
		            s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
		            s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
		            s_Data.CircleVertexBufferPtr->Thickness = circlep.thickness;
		            s_Data.CircleVertexBufferPtr->Fade = circlep.fade;
		            s_Data.CircleVertexBufferPtr->Color = circlep.color;
		            s_Data.CircleVertexBufferPtr->TextureCoord = textureCoords[i];
		            s_Data.CircleVertexBufferPtr->TextureIndex = TextureIndex;
		            s_Data.CircleVertexBufferPtr->Tiling = circlep.Tiling;
		            s_Data.CircleVertexBufferPtr->Offset = circlep.Offset;
		            s_Data.CircleVertexBufferPtr->EntityID = entityID;
		            s_Data.CircleVertexBufferPtr++;
	            }

	            s_Data.CircleIndexCount += 6;

	            s_Data.Stats.CircleCount++;
            }

            void Circle(const glm::mat4& transform, const glm::fvec4& color, float thickness, float fade, int entityID)
            {
	            Primitives::Circle circlep{
		            transform, thickness, fade, color
	            };
	            constexpr size_t circleVertexCount = 4;
	            constexpr Vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

	            // TODO: make flush for circles
	            if (s_Data.CircleIndexCount >= s_Data.MaxCircleIndices)
	            {
		            NextCircleBatch();
	            }

	            float TextureIndex = 0.0f;
	            if (circlep.texture)
	            {
		            for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		            {
			            if (*(s_Data.TextureSlots[i]) == *(circlep.texture))
			            {
				            TextureIndex = (float)i;
				            break;
			            }
		            }

		            if (TextureIndex == 0.0f)
		            {
			            if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			            {
				            NextCircleBatch();
			            }

			            TextureIndex = (float)s_Data.TextureSlotIndex;
			            s_Data.TextureSlots[s_Data.TextureSlotIndex] = circlep.texture;
			            s_Data.TextureSlotIndex++;
		            }
	            }

	            for (size_t i = 0; i < circleVertexCount; i++)
	            {
		            s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
		            s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
		            s_Data.CircleVertexBufferPtr->Thickness = circlep.thickness;
		            s_Data.CircleVertexBufferPtr->Fade = circlep.fade;
		            s_Data.CircleVertexBufferPtr->Color = circlep.color;
		            s_Data.CircleVertexBufferPtr->TextureCoord = textureCoords[i];
		            s_Data.CircleVertexBufferPtr->TextureIndex = TextureIndex;
		            s_Data.CircleVertexBufferPtr->Tiling = circlep.Tiling;
		            s_Data.CircleVertexBufferPtr->Offset = circlep.Offset;
		            s_Data.CircleVertexBufferPtr->EntityID = entityID;
		            s_Data.CircleVertexBufferPtr++;
	            }

	            s_Data.CircleIndexCount += 6;

	            s_Data.Stats.CircleCount++;
            }

            void Sprite(const Mat4& transform, SpriteRendererComponent& sprite, int entityID)
            {
            	Draw::Quad(transform, sprite.Color, sprite.Texture, sprite.Tiling, sprite.Offset, entityID);
            }

            void Line(const Vec3& p0, const Vec3& p1, const Vec4& color, int entityID)
            {
	            s_Data.LineVertexBufferPtr->Position = p0;
	            s_Data.LineVertexBufferPtr->Color = color;
	            s_Data.LineVertexBufferPtr->EntityID = entityID;
				s_Data.LineVertexBufferPtr++;
            	
            	s_Data.LineVertexBufferPtr->Position = p1;
            	s_Data.LineVertexBufferPtr->Color = color;
            	s_Data.LineVertexBufferPtr->EntityID = entityID;
				s_Data.LineVertexBufferPtr++;

				s_Data.LineVertexCount += 2;

				s_Data.Stats.LineCount++;
            }

            void Rect(const glm::fvec3& position, const glm::fvec2& size, const glm::fvec4& color, int entityID)
            {
	            Vec3 p0 = {position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z};
	            Vec3 p1 = {position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z};
	            Vec3 p2 = {position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z};
	            Vec3 p3 = {position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z};

	            Draw::Line(p0, p1, color, entityID);
	            Draw::Line(p1, p2, color, entityID);
	            Draw::Line(p2, p3, color, entityID);
	            Draw::Line(p3, p0, color, entityID);
            }

            void Rect(const glm::mat4& transform, const glm::fvec4& color, int entityID)
            {
				Vec3 RectVertices[4];
            	for (size_t	i = 0; i < 4; i++)
				{
					RectVertices[i] = transform * s_Data.QuadVertexPositions[i];
				}

				Draw::Line(RectVertices[0], RectVertices[1], color, entityID);
				Draw::Line(RectVertices[1], RectVertices[2], color, entityID);
				Draw::Line(RectVertices[2], RectVertices[3], color, entityID);
				Draw::Line(RectVertices[3], RectVertices[0], color, entityID);
            }

            float GetLineWidth()
            {
            	return s_Data.LineWidth;
            }

            void SetLineWidth(float width)
            {
            	s_Data.LineWidth = width;
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
