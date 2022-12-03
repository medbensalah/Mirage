#pragma once
#include "Mirage/Math/glmTypes.h"

struct BoxCollder2DComponent
{
	BoxCollder2DComponent() = default;
	BoxCollder2DComponent(const BoxCollder2DComponent&) = default;
	
	void* RuntimeFixture = nullptr;
	
	Vec2 Offset = {0.0f, 0.0f};
	Vec2 Size =  {0.5f, 0.5f};

	// TODO: Move into material
	
	float Density = 1.0f;
	float Friction = 0.33f;
	float Bounciness = 0.0f;
	float BouncinessThreshold = 0.5f;
};
