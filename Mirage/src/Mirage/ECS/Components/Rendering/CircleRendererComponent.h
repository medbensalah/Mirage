#pragma once

#include "Mirage/Definitions/Textures.h"
#include "Mirage/Math/glmTypes.h"
#include "Mirage/Renderer/Texture.h"

namespace Mirage
{
	struct CircleRendererComponent
	{
		float Thickness = 1.0f;
		float Fade = 0.005f;
		
		Vec4 Color{1.0f};
		Ref<Texture2D> Texture;
		Vec2 Tiling = {1.0f, 1.0f};
		Vec2 Offset = {0.0f, 0.0f};
        
		CircleRendererComponent()
		{
			Texture = Texture2D::Create(Textures::WhiteTexture);
			// uint32_t whiteTextureData = 0xffffffff;
			// Texture->SetData(&whiteTextureData, sizeof(uint32_t));
		}
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};
}
