#include "MrgPch.h"

#include "ScriptingEngine.h"
#include "ScriptGlue.h"
#include "Mirage/ECS/SceneObject.h"
#include "Mirage/ECS/Components/ScriptComponent.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/tabledefs.h"

namespace Mirage
{
	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
	{
		{"System.Single"	,		ScriptFieldType::Float},
		{"System.Double"	,		ScriptFieldType::Double},
		
		{"System.Boolean"	,		ScriptFieldType::Bool},
		{"System.Byte"		,		ScriptFieldType::Byte},
		{"System.Int16"		,		ScriptFieldType::Short},
		{"System.Int32"		,		ScriptFieldType::Int},
		{"System.Int64"		,		ScriptFieldType::Long},
		
		{"System.Char"		,		ScriptFieldType::Char},
		{"System.UInt16"	,		ScriptFieldType::Ushort},
		{"System.UInt32"	,		ScriptFieldType::Uint},
		{"System.UInt64"	,		ScriptFieldType::Ulong},
		
		{"Mirage.Vector2"	, 		ScriptFieldType::Vector2},
		{"Mirage.Vector3"	, 		ScriptFieldType::Vector3},
		{"Mirage.Vector4"	, 		ScriptFieldType::Vector4},
		
		{"Mirage.Nehavior"	, 		ScriptFieldType::Behavior},
	};
	
	
	namespace Utils
	{
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			MRG_CORE_ASSERT(stream, "Failed to open file");
			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint64_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			MRG_CORE_ASSERT(status == MONO_IMAGE_OK, mono_image_strerror(status))

			std::string assemblyName = assemblyPath.filename().string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyName.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);


				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				MRG_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			const std::string typeName = mono_type_get_name(monoType);

			const auto it = s_ScriptFieldTypeMap.find(typeName);
			if(it == s_ScriptFieldTypeMap.end())
				return ScriptFieldType::Invalid;
			return it->second;
		}

		const char* FieldTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
				case ScriptFieldType::Float:	return "float";
				case ScriptFieldType::Double:	return "double";
				
				case ScriptFieldType::Bool:		return "bool";
				case ScriptFieldType::Char:		return "char";
				case ScriptFieldType::Short:	return "short";
				case ScriptFieldType::Int:		return "int";
				case ScriptFieldType::Long:		return "long";

				case ScriptFieldType::Byte:		return "byte";
				case ScriptFieldType::Ushort:	return "ushort";
				case ScriptFieldType::Uint:		return "uint";
				case ScriptFieldType::Ulong:	return "uint";
				
				case ScriptFieldType::Vector2:	return "Vector2";
				case ScriptFieldType::Vector3:	return "Vector3";
				case ScriptFieldType::Vector4:	return "Vector4";

				case ScriptFieldType::Behavior:	return "Behavior";
			}

			return "<Invalid>";
		}
	}


	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;
		
		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		ScriptClass BehaviorClass;

		std::unordered_map<std::string, Ref<ScriptClass>> BehaviorClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> BehaviorInstances;

		//Runtime
		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;
	
	void ScriptingEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();

		LoadAssembly("Resources/Scripts/Mirage-Scripting-Core.dll");
		LoadAppAssembly("Sandbox project/Assets/Scripts/Binaries/Sandbox.dll");
		LoadAssemblyClasses();
		
		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();


		s_Data->BehaviorClass = ScriptClass("Mirage", "Behavior", true);
#if 0
		// 1. Create object
		MonoObject* instance = s_Data->BehaviorClass.Instantiate();

		// 2. Call function
		MonoMethod* printMessage = s_Data->BehaviorClass.GetMethod("PrintMessage", 0);
		s_Data->BehaviorClass.InvokeMethod(instance, printMessage);

		// 3. Call function with parameters
		MonoMethod* prinInt = s_Data->BehaviorClass.GetMethod("PrintInt", 1);
		int val = 5;
		void* params[1] = {
			&val
		};
		s_Data->BehaviorClass.InvokeMethod(instance, prinInt, params);


		MonoMethod* prinInts = s_Data->BehaviorClass.GetMethod("PrintInts", 2);
		int val1 = 15;
		int val2 = 51;
		void* params2[2] = {
			&val1, &val2
		};
		s_Data->BehaviorClass.InvokeMethod(instance, prinInts, params2);

		MonoString* monoStr = mono_string_new(s_Data->AppDomain, "Hello World from C++");
		MonoMethod* printCustomMessage = s_Data->BehaviorClass.GetMethod("PrintCustomMessage", 1);

		void* strParam = monoStr;
		s_Data->BehaviorClass.InvokeMethod(instance, printCustomMessage, &strParam);
#endif
	}

	void ScriptingEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptingEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("MirageJITRuntime");
		MRG_CORE_ASSERT(rootDomain, "Failed to initialize Mono JIT Runtime");

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;
	}

	void ScriptingEngine::ShutdownMono()
	{
		// mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		// mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	void ScriptingEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		// Create an App Domain
		s_Data->AppDomain = mono_domain_create_appdomain("MirageScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath.string());
		MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		s_Data->CoreAssemblyImage = assemblyImage;
		// Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
	}

	void ScriptingEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath.string());
		MonoImage* assemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
		s_Data->AppAssemblyImage = assemblyImage;
		// Utils::PrintAssemblyTypes(s_Data-AppAssembly);
	}

	void ScriptingEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptingEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;
		s_Data->BehaviorInstances.clear();
	}

	bool ScriptingEngine::ClassExists(const std::string& fullname)
	{
		return s_Data->BehaviorClasses.find(fullname) != s_Data->BehaviorClasses.end();
	}

	void ScriptingEngine::OnCreateBehavior(SceneObject so)
	{
		const auto& script = so.GetComponent<ScriptComponent>();
		if (ClassExists(script.ClassName))
		{
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->BehaviorClasses[script.ClassName], so);
			s_Data->BehaviorInstances[so.GetUUID()] = instance;

			instance->InvokeOnCreate();
		}
	}

	void ScriptingEngine::OnUpdateBehavior(SceneObject so, float deltaTime)
	{
		UUID uuid = so.GetUUID();
		MRG_CORE_ASSERT(s_Data->BehaviorInstances.find(uuid) != s_Data->BehaviorInstances.end(), "Behavior instance not found");

		Ref<ScriptInstance> instance = s_Data->BehaviorInstances[uuid];
		instance->InvokeOnUpdate(deltaTime);
	}

	void ScriptingEngine::OnPhysicsUpdateBehavior(SceneObject so, float deltaTime)
	{
		UUID uuid = so.GetUUID();
		MRG_CORE_ASSERT(s_Data->BehaviorInstances.find(uuid) != s_Data->BehaviorInstances.end(), "Behavior instance not found");

		Ref<ScriptInstance> instance = s_Data->BehaviorInstances[uuid];
		instance->InvokeOnPhysicsUpdate(deltaTime);
	}

	Scene* ScriptingEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptingEngine::GetBehaviorClasses()
	{
		return s_Data->BehaviorClasses;
	}
	
	void ScriptingEngine::LoadAssemblyClasses()
	{
		s_Data->BehaviorClasses.clear();
		
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		
		MonoClass* BehaviorKlass = mono_class_from_name(s_Data->CoreAssemblyImage, "Mirage", "Behavior");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
			
			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			MonoClass* klass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, name);
			if (klass == BehaviorKlass)
			{
				continue;
			}

			bool isBehavior = mono_class_is_subclass_of(klass, BehaviorKlass, 0);
			if (!isBehavior)
				continue;
		
			std::string classFullname;
			if (strlen(nameSpace) != 0)
				classFullname = fmt::format("{}.{}", nameSpace, name);
			else
				classFullname = name;
			s_Data->BehaviorClasses[classFullname] = CreateRef<ScriptClass>(nameSpace, name);

			// getting fields
			// mono_class_num_fields(klass);
			void* it = nullptr;
			MRG_CORE_TRACE("{}",classFullname);
			while (MonoClassField* field = mono_class_get_fields(klass, &it))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t fieldFlags = mono_field_get_flags(field);
				// if (fieldFlags & FIELD_ATTRIBUTE_PUBLIC)
				// {
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);
					MRG_CORE_TRACE("    {} of type {}", fieldName, Utils::FieldTypeToString(fieldType));
				// }
			}
		}
	}

	MonoImage* ScriptingEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	MonoObject* ScriptingEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);	
		return instance;
	}

#pragma region ScriptClass
	ScriptClass::ScriptClass(const std::string& nameSpace, const std::string& name, bool isCore)
		: m_NameSpace(nameSpace), m_Name(name)
	{
		if (isCore)
		{
			m_Class = mono_class_from_name(s_Data->CoreAssemblyImage, m_NameSpace.c_str(), m_Name.c_str());
		}
		else
		{
			m_Class = mono_class_from_name(s_Data->AppAssemblyImage, m_NameSpace.c_str(), m_Name.c_str());
		}
	}
	
	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptingEngine::InstantiateClass(m_Class);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int numParams)
	{
		return mono_class_get_method_from_name(m_Class, name.c_str(), numParams);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}
#pragma endregion

#pragma region ScriptClass
	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, SceneObject so)
		:m_ScriptClass(scriptClass)
	{
		m_Instance = m_ScriptClass->Instantiate();
		m_Constructor = s_Data->BehaviorClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = m_ScriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);
		m_OnPhysicsUpdateMethod = m_ScriptClass->GetMethod("OnPhysicsUpdate", 1);

		// Call behavior constructor
		{
			UUID uuid = so.GetUUID();
			void* idParam = &uuid;
			scriptClass->InvokeMethod(m_Instance, m_Constructor, &idParam);
		}
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float deltaTime)
	{
		
		if (m_OnUpdateMethod)
		{
			void* param = &deltaTime;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
		}
	}
	void ScriptInstance::InvokeOnPhysicsUpdate(float deltaTime)
	{
		if (m_OnPhysicsUpdateMethod)
		{
			void* param = &deltaTime;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnPhysicsUpdateMethod, &param);
		}
	}
#pragma endregion
}
