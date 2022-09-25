#pragma once

#include "Mirage/Renderer/OrthographicCamera.h"

#include "Mirage/Renderer/Primitives.h"

namespace Mirage
{
    namespace Renderer2D
    {
        void Init();
        void Shutdown();

        void BeginScene(const OrthographicCamera& camera);
        void EndScene();

        void Flush();

        namespace Draw
        {
            void Quad(const Primitives::Quad& quad,
                const Ref<Texture2D>& texture = nullptr,
                const Vec2& tiling = Vec2(1.0f), const Vec2& offset = Vec2{0.0f});
        }
    };
}
