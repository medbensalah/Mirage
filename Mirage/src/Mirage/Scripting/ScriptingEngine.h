#pragma once

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}

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

		static MonoObject* InstantiateClass(MonoClass* monoClass);

		friend class ScriptClass;
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;

		ScriptClass(const std::string& nameSpace, const std::string& name);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int numParams);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
		
	private:
		MonoClass* m_Class = nullptr;
		MonoObject* m_Instance = nullptr;

		std::string m_NameSpace;
		std::string m_Name;
	};
}
