using System;
using Mirage;

namespace Sandbox
{
	public class Player : Behavior
	{
		public float Speed = 0.0f;
		public float Time = 0.0f;

		private Rigidbody2D _rigidBody;

		void OnCreate()
		{
			_rigidBody = GetComponent<Rigidbody2D>();
			Speed = 0.25f;
		}

		void OnUpdate(float deltaTime)
		{
			Time += deltaTime;
		}

		void OnPhysicsUpdate(float pDeltaTime)
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

			Velocity *= Speed * pDeltaTime;

			_rigidBody.ApplyLinearImpulse(Velocity);
		}
	}
}
