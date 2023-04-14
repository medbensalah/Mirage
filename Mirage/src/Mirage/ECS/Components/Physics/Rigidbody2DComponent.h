#pragma once

class b2Body;

namespace Mirage
{
	struct Rigidbody2DComponent
	{
		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	
		enum class BodyType
		{
			Static	  = 0,
			Kinematic = 1,
			Dynamic   = 2
		};

		BodyType Type = BodyType::Static;
	

		float GravityScale = 1.0f;
		bool FixedRotation = false;
	
		b2Body* RuntimeBody = nullptr;
	};
}
