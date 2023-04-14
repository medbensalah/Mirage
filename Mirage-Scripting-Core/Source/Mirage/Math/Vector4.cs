using System;

namespace Mirage
{
	public struct Vector4
	{
		public float x;
		public float y;
		public float z;
		public float w;

		public static Vector4 Zero => new Vector4(0, 0, 0, 0);
		public static Vector4 One => new Vector4(1, 1, 1, 1);

		public Vector4(float scalar)
		{
			x = y = z = w = scalar;
		}

		public Vector4(float x, float y, float z, float w)
		{
			this.x = x;
			this.y = y;
			this.z = z;
			this.w = w;
		}

		public Vector4(Vector2 v2, float zVal, float wVal)
		{
			x = v2.x;
			y = v2.y;
			z = zVal;
			w = wVal;
		}

		public Vector4(Vector3 v3, float wVal)
		{
			x = v3.x;
			y = v3.y;
			z = v3.z;
			w = wVal;
		}

		public Vector4(Vector4 v4)
		{
			x = v4.x;
			y = v4.y;
			z = v4.z;
			w = v4.w;
		}

		public static Vector4 operator +(Vector4 vec1, float scalar)
		{
			return new Vector4(vec1.x + scalar, vec1.y + scalar, vec1.z + scalar, vec1.w + scalar);
		}

		public static Vector4 operator +(Vector4 vec1, Vector4 vec2)
		{
			return new Vector4(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.w);
		}

		public static Vector4 operator -(Vector4 vec1, float scalar)
		{
			return new Vector4(vec1.x - scalar, vec1.y - scalar, vec1.z - scalar, vec1.w - scalar);
		}

		public static Vector4 operator -(Vector4 vec1, Vector4 vec2)
		{
			return new Vector4(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z, vec1.w - vec2.w);
		}

		public static Vector4 operator *(Vector4 vector, float scalar)
		{
			return new Vector4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
		}

		public static Vector4 operator /(Vector4 vector, float scalar)
		{
			return new Vector4(vector.x / scalar, vector.y / scalar, vector.z / scalar, vector.w / scalar);
		}


		public static bool operator ==(Vector4 vec1, Vector4 vec2)
		{
			return Math.Abs(vec1.x - vec2.x) < Consts.FLOAT_TOLERANCE &&
			       Math.Abs(vec1.y - vec2.y) < Consts.FLOAT_TOLERANCE &&
			       Math.Abs(vec1.z - vec2.z) < Consts.FLOAT_TOLERANCE &&
			       Math.Abs(vec1.w - vec2.w) < Consts.FLOAT_TOLERANCE;
		}

		public static bool operator !=(Vector4 vec1, Vector4 vec2)
		{
			return Math.Abs(vec1.x - vec2.x) > Consts.FLOAT_TOLERANCE ||
			       Math.Abs(vec1.y - vec2.y) > Consts.FLOAT_TOLERANCE ||
			       Math.Abs(vec1.z - vec2.z) > Consts.FLOAT_TOLERANCE ||
			       Math.Abs(vec1.w - vec2.w) > Consts.FLOAT_TOLERANCE;
		}

		public override string ToString()
		{
			return string.Format("({0}, {1}, {2}, {3})", x, y, z, w);
		}
	}
}
