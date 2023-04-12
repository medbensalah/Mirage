using System;
using System.Collections;
using System.Runtime.CompilerServices;

namespace Mirage
{
	public struct Vector3
	{
		public float x, y, z;
		
		public static Vector3 Zero => new Vector3(0, 0, 0);
		public static Vector3 Up => new Vector3(0, 1, 0);
		public static Vector3 Down => new Vector3(0, -1, 0);
		public static Vector3 Left => new Vector3(-1, 0, 0);
		public static Vector3 Right => new Vector3(1, 0, 0);
		public static Vector3 Forward => new Vector3(0, 0, 1);
		public static Vector3 Back => new Vector3(0, 0, -1);
		
		
		
		public Vector3(float scalar)
		{
			x = y = z = scalar;
		}
		
		public Vector3(float x, float y, float z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}
		
		public static Vector3 operator*(Vector3 vector, float scalar)
		{
			return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
		}
		public static Vector3 operator+(Vector3 vec1, float scalar)
		{
			return new Vector3(vec1.x + scalar, vec1.y + scalar, vec1.z + scalar);
		}
		public static Vector3 operator+(Vector3 vec1, Vector3 vec2)
		{
			return new Vector3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
		}
		
		public override string ToString()
		{
			return string.Format("({0}, {1}, {2})", x, y, z);
		}
	}

	public static class InternalCalls
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void CppLogFunc(string text, int parameter);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void CppLogFuncVec3(ref Vector3 vec);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SO_GetPosition(ulong uuid, out Vector3 vec);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void SO_SetPosition(ulong uuid, ref Vector3 vec);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Input_IsKeyDown(KeyCode keycode);
	}
	public class Behavior
	{
		protected Behavior()
		{
			Console.WriteLine("Behavior created with no uuid");
			_uuid = 0;
		}
		internal Behavior(ulong uuid)
		{
			Console.WriteLine("Behavior created with uuid {0}", uuid);
			_uuid = uuid;
		}

		public readonly ulong _uuid;

		public Vector3 Position
		{
			get
			{
				InternalCalls.SO_GetPosition(_uuid, out Vector3 vec);
				return vec;
			}
			set
			{
				InternalCalls.SO_SetPosition(_uuid, ref value);
			}
		}
	}
}
