#pragma once

#include "Mirage/Math/glmTypes.h"

namespace Mirage
{
    
    struct SpriteRendererComponent
    {
        Vec4 Color{1.0f};
        
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const Vec4& color)
            : Color(color)
        {
        }
    };
}