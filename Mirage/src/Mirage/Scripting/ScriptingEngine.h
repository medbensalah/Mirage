#pragma once
#include <map>

#include "Mirage/Core/UUID.h"

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
typedef struct _MonoClassField MonoClassField;
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

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;
		MonoClassField* ClassField;
	};
	
	class ScriptClass
	{
	public:
		ScriptClass() = default;

		ScriptClass(const std::string& nameSpace, const std::string& name, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int numParams);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		const std::map<std::string, ScriptField>& GetPublicFields() { return m_PublicFields; };
		const std::map<std::string, ScriptField>& GetPrivateFields() { return m_PrivateFields; };

	private:
		MonoClass* m_Class = nullptr;
		MonoObject* m_Instance = nullptr;

		std::string m_NameSpace;
		std::string m_Name;

		std::map<std::string, ScriptField> m_PublicFields;
		std::map<std::string, ScriptField> m_PrivateFields;

		friend class ScriptingEngine;
	};

	class ScriptInstance
	{
	public :
		ScriptInstance(Ref<ScriptClass> scriptClass, SceneObject so);

		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);
		void InvokeOnPhysicsUpdate(float deltaTime);

		Ref<ScriptClass> GetScriptClass() {return m_ScriptClass;}
		

		template <typename T>
		T GetFieldValue(const std::string& name)
		{
			char s_FieldValueBuffer[sizeof(T)];
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();	
			return *(T*)s_FieldValueBuffer;
		}
		
		template <typename T>
		void SetFieldValue(const std::string& name, const T& value)
		{
			SetFieldValueInternal(name, &value);
		}
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
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
		static Ref<ScriptInstance> GetSOScriptInstance(UUID id);
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
