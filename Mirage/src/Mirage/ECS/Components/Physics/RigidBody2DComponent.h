#pragma once

struct RigidBody2DComponent
{
	RigidBody2DComponent() = default;
	RigidBody2DComponent(const RigidBody2DComponent&) = default;
	
	enum class SimulationType
	{
		Static	  = 0,
		Kinematic = 2,
		Dynamic   = 1
	};

	SimulationType Type = SimulationType::Static;
	
	bool FixedRotation = false;
	void* RuntimeBody = nullptr;
};
