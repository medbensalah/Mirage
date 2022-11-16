#pragma once

#include "Mirage/Math/glmTypes.h"
#include "Mirage/Renderer/Texture.h"

namespace Mirage
{
    
    struct SpriteRendererComponent
    {
        Vec4 Color{1.0f};
        Ref<Texture2D> Texture = nullptr;
        Vec2 Tiling = {1.0f, 1.0f};
        Vec2 Offset = {0.0f, 0.0f};
        
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const Vec4& color)
            : Color(color)
        {
        }
    };
}
