using System;
using Mirage;

namespace Sandbox
{
	public class Player : Behavior
	{
		float Speed = 2.0f;
		
		void OnCreate()
		{
			Console.WriteLine("Player created");
		}
		void OnUpdate(float deltaTime)
		{
		}
		void OnPhysicsUpdate(float pDeltaTime)
		{
			Vector3 Velocity = Vector3.Zero;
			
			if (Input.IsKeyDown(KeyCode.W))
				Velocity.y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				Velocity.y = -1.0f;
			
			if (Input.IsKeyDown(KeyCode.A))
				Velocity.x = -1.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				Velocity.x = 1.0f;
			
			transform.Position += Velocity * pDeltaTime;
		}
	}
}
