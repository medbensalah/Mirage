using System;

namespace Mirage
{
	public struct Vector2
	{
		public override bool Equals(object obj)
		{
			return obj is Vector2 other && Equals(other);
		}

		public override int GetHashCode()
		{
			unchecked
			{
				return (x.GetHashCode() * 397) ^ y.GetHashCode();
			}
		}

		public float x;
		public float y;

		public static Vector2 Zero => new Vector2(0, 0);
		public static Vector2 Up => new Vector2(0, 1);
		public static Vector2 Down => new Vector2(0, -1);
		public static Vector2 Left => new Vector2(-1, 0);
		public static Vector2 Right => new Vector2(1, 0);
		
		public Vector2(float scalar)
		{
			x = y = scalar;
		}

		public Vector2(float x, float y)
		{
			this.x = x;
			this.y = y;
		}

		public Vector2(Vector3 v3)
		{
			this.x = v3.x;
			this.y = v3.y;
		}


		public static Vector2 operator +(Vector2 vec1, float scalar)
		{
			return new Vector2(vec1.x + scalar, vec1.y + scalar);
		}
		public static Vector2 operator +(Vector2 vec1, Vector2 vec2)
		{
			return new Vector2(vec1.x + vec2.x, vec1.y + vec2.y);
		}
		public static Vector2 operator -(Vector2 vec1, float scalar)
		{
			return new Vector2(vec1.x - scalar, vec1.y - scalar);
		}
		public static Vector2 operator -(Vector2 vec1, Vector2 vec2)
		{
			return new Vector2(vec1.x - vec2.x, vec1.y - vec2.y);
		}
		public static Vector2 operator *(Vector2 vector, float scalar)
		{
			return new Vector2(vector.x * scalar, vector.y * scalar);
		}
		public static Vector2 operator /(Vector2 vector, float scalar)
		{
			return new Vector2(vector.x / scalar, vector.y / scalar);
		}

		
		public static bool operator ==(Vector2 vec1, Vector2 vec2)
		{
			return Math.Abs(vec1.x - vec2.x) < Consts.FLOAT_TOLERANCE && Math.Abs(vec1.y - vec2.y) < Consts.FLOAT_TOLERANCE;
		}
		public static bool operator !=(Vector2 vec1, Vector2 vec2)
		{
			return Math.Abs(vec1.x - vec2.x) > Consts.FLOAT_TOLERANCE || Math.Abs(vec1.y - vec2.y) > Consts.FLOAT_TOLERANCE;
		}
		
		public override string ToString()
		{
			return string.Format("({0}, {1})", x, y);
		}
	}
}
