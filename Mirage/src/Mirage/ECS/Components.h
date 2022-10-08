#include "Mirage/Core/Math.h"

namespace Mirage
{
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : Tag(tag)
        {
        }
    };
    
    struct TransformComponent
    {
        Mat4 Trannsform = Mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const Mat4& transform)
            : Trannsform(transform)
        {
        }

        operator const Mat4&() const { return Trannsform; }
        operator Mat4&() { return Trannsform; }
    };
    
    struct SpriteRendererComponent
    {
        Vec4 Color{1.0f};
        
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const Vec4& color)
            : Color(color)
        {
        }
    };
}
