using System;
using System.Collections;
using System.Runtime.CompilerServices;

namespace Mirage
{
	public struct Vector3
	{
		public float x, y, z;
		
		public Vector3(float x, float y, float z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}
	}

	public static class InternalCalls
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void CppLogFunc(string text, int parameter);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal extern static void CppLogFuncVec3(ref Vector3 vec);
	}
	public class Main
	{
		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main constructor");
			
			InternalCalls.CppLogFunc("AABB", 123);
			
			Vector3 pos = new Vector3(1, 2, 3);
			InternalCalls.CppLogFuncVec3(ref pos);
		}

		public void PrintMessage()
		{
			Console.WriteLine("Hello from C# Main");
		}

		public void PrintInt(int val)
		{
			Console.WriteLine($"{val} from C# Main");
		}

		public void PrintInts(int val1, int val2)
		{
			Console.WriteLine($"{val1} and {val2} from C# Main");
		}
		
		public void PrintCustomMessage(string message)
		{
			Console.WriteLine($"{message} from C# Main");
		}

		~Main()
		{
			Console.WriteLine("Main destructor");
		}
	}
}
