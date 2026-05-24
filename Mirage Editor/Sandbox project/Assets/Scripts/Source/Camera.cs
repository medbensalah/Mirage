using System;
using Mirage;

namespace Sandbox
{
	public class Camera : Behavior
	{
		public float Speed = 5.0f;

		void OnCreate()
		{
		}

		void OnUpdate(float deltaTime)
		{
			Vector3 Velocity = Vector3.Zero;
			Vector2 a = new Vector2(1, 2);

			if (Input.IsKeyDown(KeyCode.W))
				Velocity.y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				Velocity.y = -1.0f;

			if (Input.IsKeyDown(KeyCode.A))
				Velocity.x = -1.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				Velocity.x = 1.0f;

			if (Velocity != Vector3.Zero)
			{
				Velocity *= Speed * deltaTime;
				transform.Position += Velocity;
			}
		}

		void OnPhysicsUpdate(float pDeltaTime)
		{
		}
	}
}
