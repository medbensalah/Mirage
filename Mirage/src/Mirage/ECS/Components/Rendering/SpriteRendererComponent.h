#pragma once

#include "Mirage/Definitions/Textures.h"
#include "Mirage/Math/glmTypes.h"
#include "Mirage/Renderer/Texture.h"

namespace Mirage
{
    struct SpriteRendererComponent
    {
        Vec4 Color{1.0f};
        Ref<Texture2D> Texture;
        Vec2 Tiling = {1.0f, 1.0f};
        Vec2 Offset = {0.0f, 0.0f};
        
        SpriteRendererComponent()
        {
	        Texture = Texture2D::Create(Textures::WhiteTexture);
        	// uint32_t whiteTextureData = 0xffffffff;
        	// Texture->SetData(&whiteTextureData, sizeof(uint32_t));
        }
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const Vec4& color, const Ref<Texture2D>& tex = nullptr, const Vec2& tiling = {1.0f, 1.0f}, const Vec2& offset = {0.0f, 0.0f})
            : Color(color), Texture(tex), Tiling(tiling), Offset(offset)
        {
	        if (tex == nullptr)
	        {
		        Texture = Texture2D::Create(Textures::WhiteTexture);
	        	// uint32_t whiteTextureData = 0xffffffff;
	        	// Texture->SetData(&whiteTextureData, sizeof(uint32_t));
	        }
        }
    };
}
