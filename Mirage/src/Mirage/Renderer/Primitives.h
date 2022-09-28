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
                Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
                Ref<Texture2D> texture;

                Quad() = default;
                
            } Quad;
        }
    }
}
