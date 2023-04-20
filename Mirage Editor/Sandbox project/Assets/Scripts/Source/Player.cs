﻿using System;
using Mirage;

namespace Sandbox
{
	public class Player : Behavior
	{
		public float Speed = 50.0f;
		public bool Bool;
		public byte Byte;
		public short Short;
		public int Int;
		public long Long;
		public float Float;
		public double Double;
		public char Char;
		public ushort UShort;
		public uint UInt;
		public ulong ULong;
		public Vector2 Vector2;
		public Vector3 Vector3;
		public Vector4 Vector4;
		public Behavior Behavior;
		public Camera Camera;
		
		
		private Rigidbody2D _rigidBody;
		
		void OnCreate()
		{
			_rigidBody = GetComponent<Rigidbody2D>();
		}
		void OnUpdate(float deltaTime)
		{
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
