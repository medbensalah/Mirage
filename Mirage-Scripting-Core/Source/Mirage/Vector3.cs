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
		
		public override string ToString()
		{
			return string.Format("({0}, {1}, {2})", x, y, z);
		}
	}
}
