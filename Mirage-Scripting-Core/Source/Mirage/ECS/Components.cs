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
}
