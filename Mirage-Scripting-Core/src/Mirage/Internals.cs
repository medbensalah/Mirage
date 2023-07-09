using System;
using System.Runtime.CompilerServices;

namespace Mirage
{
	internal class Internals
	{
		#region SceneObject
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool SO_HasComponent(ulong uuid, Type type);
		#endregion
		

		#region Transform
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Transform_GetPosition(ulong uuid, out Vector3 vec);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Transform_SetPosition(ulong uuid, ref Vector3 vec);
		#endregion

		#region Rigidbody
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Rigidbody2D_ApplyLinearImpulse(ulong uuid, ref Vector2 impulse, ref Vector2 worldPoint, bool wake);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Rigidbody2D_ApplyLinearImpulseToCenter(ulong uuid, ref Vector2 impulse, bool wake);
		#endregion
		
		
		#region Input
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Input_IsKeyDown(KeyCode keycode);
		#endregion
	}
}
