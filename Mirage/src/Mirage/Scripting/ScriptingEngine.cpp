#include "MrgPch.h"

#include "ScriptingEngine.h"
#include "ScriptGlue.h"

#include "Mirage/Math/glmTypes.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Mirage
{
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
	}



	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		ScriptClass SceneObjectClass;
	};

	static ScriptEngineData* s_Data = nullptr;


	void ScriptingEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();

		LoadAssembly("Resources/Scripts/Mirage-Scripting-Core.dll");

		ScriptGlue::RegisterFunctions();

		s_Data->SceneObjectClass = ScriptClass("Mirage", "SceneObject");

		// 1. Create object
		MonoObject* instance = s_Data->SceneObjectClass.Instantiate();

		// 2. Call function
		MonoMethod* printMessage = s_Data->SceneObjectClass.GetMethod("PrintMessage", 0);
		s_Data->SceneObjectClass.InvokeMethod(instance, printMessage);
		mono_runtime_invoke(printMessage, instance, nullptr, nullptr);

		// 3. Call function with parameters
		MonoMethod* prinInt = s_Data->SceneObjectClass.GetMethod("PrintInt", 1);
		int val = 5;
		void* params[1] = {
			&val
		};
		s_Data->SceneObjectClass.InvokeMethod(instance, prinInt, params);


		MonoMethod* prinInts = s_Data->SceneObjectClass.GetMethod("PrintInts", 2);
		int val1 = 15;
		int val2 = 51;
		void* params2[2] = {
			&val1, &val2
		};
		s_Data->SceneObjectClass.InvokeMethod(instance, prinInts, params2);

		MonoString* monoStr = mono_string_new(s_Data->AppDomain, "Hello World from C++");
		MonoMethod* printCustomMessage = s_Data->SceneObjectClass.GetMethod("PrintCustomMessage", 1);

		void* strParam = monoStr;
		s_Data->SceneObjectClass.InvokeMethod(instance, printCustomMessage, &strParam);
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

	MonoObject* ScriptingEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}
	
	ScriptClass::ScriptClass(const std::string& nameSpace, const std::string& name)
		: m_NameSpace(nameSpace), m_Name(name)
	{
		m_Class = mono_class_from_name(s_Data->CoreAssemblyImage, m_NameSpace.c_str(), m_Name.c_str());
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
}
