#pragma once

#include "Mirage/Core/Math.h"

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