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
                Vec3 position;
                Vec3 rotation;
                Vec3 scale;
                Vec4 color;
                Vec2 tiling;
                Vec2 offset;
                Ref<Texture2D> texture;

                Quad()
                {
                }
            } Quad;
        }
    }
}
