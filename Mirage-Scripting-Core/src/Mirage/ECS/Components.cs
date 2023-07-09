namespace Mirage
{
	public abstract class Component
	{
		public Behavior Behavior { get; internal set; }
		
	}
	
	public class Transform : Component
	{
		public Vector3 Position
		{
			get
			{
				Internals.Transform_GetPosition(Behavior._uuid, out Vector3 vec);
				return vec;
			}
			set => Internals.Transform_SetPosition(Behavior._uuid, ref value);
		}
	}
	
	public class Rigidbody2D : Component
	{
		public void ApplyLinearImpulse(Vector2 impulse, bool wake = true)
		{
			Internals.Rigidbody2D_ApplyLinearImpulseToCenter(Behavior._uuid, ref impulse, wake);
		}
		public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake = true)
		{
			Internals.Rigidbody2D_ApplyLinearImpulse(Behavior._uuid, ref impulse, ref worldPosition, wake);
		}
	}
}
