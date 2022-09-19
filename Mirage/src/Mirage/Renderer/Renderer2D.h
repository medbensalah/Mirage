#pragma once

#include "OrthographicCamera.h"

namespace Mirage
{
    namespace Renderer2D
    {
         void Init();
         void Shutdown();

         void BeginScene(const OrthographicCamera& camera);
         void EndScene();

        namespace Draw
        {
            void Quad(
                const Vec2& position = Vec2{0.0f},
                const Vec2& scale = Vec2{1.0f},
                float rotation = 0.0f,
                const Vec4& color = Vec4{1.0f, 1.0f, 1.0f, 1.0f}
            );
            void Quad(
                const Vec3& position = Vec3{0.0f},
                const Vec3& scale = Vec3{1.0f},
                const Vec3& rotation = Vec3{1.0f},
                const Vec4& color = Vec4{1.0f, 1.0f, 1.0f, 1.0f}
            );
        }
    };
}
