using System;

namespace Sandbox
{
	public class Player
	{
		void OnCreate()
		{
			Console.WriteLine("Player created");
		}
		
		void OnUpdate(float deltaTime)
		{
			Console.WriteLine("Player updated {deltaTime}");
		}
	}
}
