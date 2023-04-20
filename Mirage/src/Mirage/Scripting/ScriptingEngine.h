#pragma once

namespace Mirage
{
	class Scene;
	class SceneObject;
}

extern "C" {
typedef struct _MonoClass MonoClass;
typedef struct _MonoObject MonoObject;
typedef struct _MonoMethod MonoMethod;
typedef struct _MonoAssembly MonoAssembly;
typedef struct _MonoImage MonoImage;
}

namespace Mirage
{
	enum class ScriptFieldType
	{
		Invalid = 0,
		Float, Double,
		Char, Short, Int, Long, Bool,
		Byte, Ushort, Uint, Ulong,
		Vector2, Vector3, Vector4,
		Behavior,
	};
	
	class ScriptClass
	{
	public:
		ScriptClass() = default;

		ScriptClass(const std::string& nameSpace, const std::string& name, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int numParams);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

	private:
		MonoClass* m_Class = nullptr;
		MonoObject* m_Instance = nullptr;

		std::string m_NameSpace;
		std::string m_Name;
	};

	class ScriptInstance
	{
	public :
		ScriptInstance(Ref<ScriptClass> scriptClass, SceneObject so);

		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);
		void InvokeOnPhysicsUpdate(float deltaTime);

	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;

		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
		MonoMethod* m_OnPhysicsUpdateMethod = nullptr;
	};

	class ScriptingEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);
		static void OnRuntimeStart(class Scene* scene);
		static void OnRuntimeStop();

		static bool ClassExists(const std::string& fullname);
		static void OnCreateBehavior(SceneObject so);
		static void OnUpdateBehavior(SceneObject so, float deltaTime);
		static void OnPhysicsUpdateBehavior(SceneObject so, float deltaTime);

		static Scene* GetSceneContext();
		static MonoImage* GetCoreAssemblyImage();

		static std::unordered_map<std::string, Ref<ScriptClass>> GetBehaviorClasses();

	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);

		static void LoadAssemblyClasses();
		
		friend class ScriptClass;
		friend class ScriptGlue;
	};
}
