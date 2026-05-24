namespace Mirage
{
	public class Input
	{
		public static bool IsKeyDown(KeyCode keycode)
		{
			return Internals.Input_IsKeyDown(keycode);
		}
	}
}
