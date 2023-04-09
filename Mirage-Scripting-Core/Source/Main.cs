using System;

namespace MirageScriptingCore
{
	public class Main
	{
		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main constructor");
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
