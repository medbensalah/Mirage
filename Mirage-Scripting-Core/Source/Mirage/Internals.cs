using System;
using System.Runtime.CompilerServices;

namespace Mirage
{
	internal class Internals
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool SO_HasComponent(ulong uuid, Type type);
		
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Transform_GetPosition(ulong uuid, out Vector3 vec);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Transform_SetPosition(ulong uuid, ref Vector3 vec);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Input_IsKeyDown(KeyCode keycode);
	}
}
