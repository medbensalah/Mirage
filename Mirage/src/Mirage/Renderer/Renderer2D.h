#pragma once

#include "Mirage/Renderer/Camera.h"
#include "Mirage/Renderer/OrthographicCamera.h"

#include "Mirage/Renderer/Primitives.h"

namespace Mirage
{
    namespace Renderer2D
    {        
        void Init();
        void Shutdown();

        void BeginScene(const Camera& camera, const Mat4& transform);
        void BeginScene(const OrthographicCamera& camera);
        void EndScene();

        void Flush();

        namespace Draw
        {
            void Quad(const Mat4& transform, const Vec4& color, const Ref<Texture2D> texture = nullptr, const Vec2& tiling = Vec2(1.0f), const Vec2& offset = Vec2{0.0f});
            void Quad(const Primitives::Quad& quad, const Vec2& tiling = Vec2(1.0f), const Vec2& offset = Vec2{0.0f});
        }

        
        struct Stats
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetVertexCount() { return QuadCount * 4; }
            uint32_t GetIndexCount() { return QuadCount * 6; }
        };
        
        Stats GetStats();
        void ResetStats();
    };
}
