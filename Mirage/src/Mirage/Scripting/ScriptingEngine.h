#pragma once

namespace Mirage
{
	class ScriptingEngine
	{
	public:
		static void Init();
		static void Shutdown();

	private:
		static void InitMono();
		static void ShutdownMono();
		
	};
}
