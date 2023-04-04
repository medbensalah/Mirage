#pragma once

#include "Mirage/ECS/Components/Rendering/CircleRendererComponent.h"
#include "Mirage/ECS/Components/Rendering/SpriteRendererComponent.h"
#include "Mirage/Renderer/Camera.h"
#include "Mirage/Renderer/OrthographicCamera.h"

#include "Mirage/Renderer/Primitives.h"
#include "Mirage/Renderer/EditorCamera.h"

namespace Mirage
{
    namespace Renderer2D
    {        
        void Init();
        void Shutdown();

        void BeginScene(const Camera& camera, const Mat4& transform);
        void BeginScene(const EditorCamera& camera);
        void BeginScene(const OrthographicCamera& camera); //TODO: Remove this
    	
        void StartBatch();
        void StartQuadBatch();
        void StartCircleBatch();
        void StartLineBatch();

    	void EndScene();
    	
        void FlushQuad();
        void FlushCircle();
        void FlushLine();
        void NextQuadBatch();
        void NextCircleBatch();
        void NextLineBatch();

        namespace Draw
        {
            void Quad(const Mat4& transform, const Vec4& color, const Ref<Texture2D> texture = nullptr, const Vec2& tiling = Vec2(1.0f), const Vec2& offset = Vec2{0.0f}, int entityID = -1);
            void Quad(const Primitives::Quad& quad, int entityID = -1);
            void Circle(const Mat4& transform, CircleRendererComponent& circle, int entityID);
            void Circle(const Mat4& transform, const Vec4& color, float thickness, float fade, int entityID);
            void Sprite(const Mat4& transform, SpriteRendererComponent& sprite, int entityID);
            void Line(const Vec3& p0, const Vec3& p1, const Vec4& color, int entityID);
            void Rect(const Vec3& position, const Vec2& size, const Vec4& color, int entityID);
            void Rect(const Mat4& transform, const Vec4& color, int entityID);

        	float GetLineWidth();
        	void SetLineWidth(float width);
        }

        
        struct Stats
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;
            uint32_t CircleCount = 0;
            uint32_t LineCount = 0;

            uint32_t GetVertexCount() const { return (QuadCount + CircleCount) * 4 + LineCount * 2; }
            uint32_t GetIndexCount() const { return (QuadCount + CircleCount) * 6 ; }
        };
        
        Stats GetStats();
        void ResetStats();
    };
}
