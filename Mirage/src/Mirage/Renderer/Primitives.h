#pragma once

#include "Mirage/Renderer/Texture.h"

namespace Mirage
{
    namespace Renderer2D
    {
        namespace Primitives
        {
            typedef struct Quad
            {
                Mat4 transform;
                Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
                Ref<Texture2D> texture;
                Vec2 Tiling = { 1.0f, 1.0f };
                Vec2 Offset = { 0.0f, 0.0f };

                Quad() = default;
                
            } Quad;
            typedef struct Circle
            {
                Mat4 transform;
            	float thickness = 1.0f;
            	float fade = 0.005f;
                Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
                Ref<Texture2D> texture;
                Vec2 Tiling = { 1.0f, 1.0f };
                Vec2 Offset = { 0.0f, 0.0f };

                Circle() = default;
                
            } Circle;
        }
    }
}
