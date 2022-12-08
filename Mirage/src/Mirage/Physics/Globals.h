#pragma once

namespace Mirage
{
	namespace Physics2D
	{
		static bool UpdateEveryFrame = true;
		static float TimeStep = 1.0f / 60.0f;
		
		static float Gravity = -9.78f;

		static int32 VelocityIterations = 8;
		static int32 PositionIterations = 3;
	}
}
