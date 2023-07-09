using System;

namespace Mirage
{
	public struct Vector3
	{
		public float x;
		public float y;
		public float z;

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

		public Vector3(Vector2 v2, float zVal = 0)
		{
			x = v2.x;
			y = v2.y;
			z = zVal;
		}

		public Vector3(Vector3 v3)
		{
			this.x = v3.x;
			this.y = v3.y;
			this.z = v3.z;
		}

		public Vector3(Vector4 v4)
		{
			this.x = v4.x;
			this.y = v4.y;
			this.z = v4.z;
		}


		public static Vector3 operator +(Vector3 vec1, float scalar)
		{
			return new Vector3(vec1.x + scalar, vec1.y + scalar, vec1.z + scalar);
		}

		public static Vector3 operator +(Vector3 vec1, Vector3 vec2)
		{
			return new Vector3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
		}


		public static Vector3 operator -(Vector3 vec1, float scalar)
		{
			return new Vector3(vec1.x - scalar, vec1.y - scalar, vec1.z - scalar);
		}

		public static Vector3 operator -(Vector3 vec1, Vector3 vec2)
		{
			return new Vector3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
		}


		public static Vector3 operator *(Vector3 vector, float scalar)
		{
			return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
		}

		public static Vector3 operator /(Vector3 vector, float scalar)
		{
			return new Vector3(vector.x / scalar, vector.y / scalar, vector.z / scalar);
		}
		
		public static bool operator ==(Vector3 vec1, Vector3 vec2)
		{
			return Math.Abs(vec1.x - vec2.x) < Consts.FLOAT_TOLERANCE && Math.Abs(vec1.y - vec2.y) < Consts.FLOAT_TOLERANCE && Math.Abs(vec1.z - vec2.z) < Consts.FLOAT_TOLERANCE;
		}
		public static bool operator !=(Vector3 vec1, Vector3 vec2)
		{
			return Math.Abs(vec1.x - vec2.x) > Consts.FLOAT_TOLERANCE || Math.Abs(vec1.y - vec2.y) > Consts.FLOAT_TOLERANCE || Math.Abs(vec1.z - vec2.z) > Consts.FLOAT_TOLERANCE;
		}

		public static implicit operator Vector2(Vector3 v) => new Vector2(v.x, v.y);

		public override string ToString()
		{
			return string.Format("({0}, {1}, {2})", x, y, z);
		}
	}
}
