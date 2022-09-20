#pragma once

#include "Texture.h"

namespace Mirage
{
    namespace Renderer2D
    {
        namespace Primitives
        {
            typedef struct Quad
            {
                const Vec3& position;
                const Vec3& rotation;
                const Vec3& scale;
                const Vec4& color;
                const Vec2& tiling;
                const Vec2& offset;
                const Ref<Texture2D>& texture;

                Quad(
                    const Vec3& position = Vec3{0.0f},
                    const Vec3& rotation = Vec3{0.0f},
                    const Vec3& scale = Vec3{1.0f},
                    const Vec4& color = Vec4{1.0f, 1.0f, 1.0f, 1.0f},
                    const Ref<Texture2D>& texture = Texture2D::GetDefaultTexture(),
                    const Vec2& tiling = Vec2{1.0f, 1.0f},
                    const Vec2& offset = Vec2{0.0f, 0.0f}
                )
                    : position(position), rotation(rotation), scale(scale), color(color), tiling(tiling), offset(offset), texture(texture)
                {
                }

                Quad(
                    const Vec2& position = Vec2{0.0f},
                    float rotation = 0.0f,
                    const Vec2& scale = Vec3{1.0f},
                    const Vec4& color = Vec4{1.0f, 1.0f, 1.0f, 1.0f},
                    const Ref<Texture2D>& texture = Texture2D::GetDefaultTexture(),
                    const Vec2& tiling = Vec2{1.0f, 1.0f},
                    const Vec2& offset = Vec2{0.0f, 0.0f}
                )
                    : position(Vec3{position.x, position.y, 0.0f}), rotation(Vec3{0.0f, 0.0f, rotation}),
                      scale(Vec3{scale.x, scale.y, 1.0f}), color(color), tiling(tiling), offset(offset), texture(texture)
                {
                }

                Quad(
                    const Vec3& position = Vec3{0.0f},
                    const Vec3& rotation = Vec3{0.0f},
                    const Vec3& scale = Vec3{1.0f},
                    const Ref<Texture2D>& texture = Texture2D::GetDefaultTexture(),
                    const Vec2& tiling = Vec2{1.0f, 1.0f},
                    const Vec2& offset = Vec2{0.0f, 0.0f}
                )
                    : position(position), rotation(rotation), scale(scale), color(Vec4{1.0f}), tiling(tiling), offset(offset), texture(texture)
                {
                }

                Quad(
                    const Vec2& position = Vec2{0.0f},
                    float rotation = 0.0f,
                    const Vec2& scale = Vec3{1.0f},
                    const Ref<Texture2D>& texture = Texture2D::GetDefaultTexture(),
                    const Vec2& tiling = Vec2{1.0f, 1.0f},
                    const Vec2& offset = Vec2{0.0f, 0.0f}
                )
                    : position(Vec3{position.x, position.y, 0.0f}), rotation(Vec3{0.0f, 0.0f, rotation}),
                      scale(Vec3{scale.x, scale.y, 1.0f}), color(Vec4{1.0f}), tiling(tiling), offset(offset), texture(texture)
                {
                }
            } Quad;
        }
    }
}
