#pragma once
#include "Mirage/Math/glmTypes.h"

namespace Mirage
{
	struct CircleCollider2DComponent
	{
		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	
		void* RuntimeFixture = nullptr;

		float Radius = 0.5f;
		Vec2 Offset = {0.0f, 0.0f};
		
		// TODO: Move into material
	
		float Density = 1.0f;
		float Friction = 0.33f;
		float Bounciness = 0.0f;
		float BouncinessThreshold = 0.5f;
	};
}
