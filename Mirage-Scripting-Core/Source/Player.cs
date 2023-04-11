using System;
using Mirage;

namespace Sandbox
{
	public class Player : Behavior
	{
		void OnCreate()
		{
			Console.WriteLine("Player created");
		}
		
		void OnUpdate(float deltaTime)
		{
			Console.WriteLine($"Player updated: {deltaTime}");
		}
	}
}
