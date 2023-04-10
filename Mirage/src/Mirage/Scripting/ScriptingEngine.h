#pragma once

namespace Mirage
{
	class ScriptingEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);

	private:
		static void InitMono();
		static void ShutdownMono();
		
	};
}
