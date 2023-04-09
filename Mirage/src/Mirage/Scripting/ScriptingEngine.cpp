#include "MrgPch.h"

#include "ScriptingEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Mirage
{
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;
	
	void ScriptingEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();
	}

	void ScriptingEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
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
		uint32_t size = end - stream.tellg();
    
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

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		MRG_CORE_ASSERT(status == MONO_IMAGE_OK, mono_image_strerror(status))

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
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
	
	void ScriptingEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");
		
		MonoDomain* rootDomain = mono_jit_init("MirageJITRuntime");
		MRG_CORE_ASSERT(rootDomain, "Failed to initialize Mono JIT Runtime");

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;

		// Create an App Domain
		s_Data->AppDomain = mono_domain_create_appdomain("MirageScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);



		
		s_Data->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/Mirage-Scripting-Core.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);
		
		MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* monoClass = mono_class_from_name(assemblyImage, "MirageScriptingCore", "Main");
		// 1. Create object
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		
		// 2. Call function
		MonoMethod* printMessage = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		mono_runtime_invoke(printMessage, instance, nullptr, nullptr);
		
		// 3. Call function with parameters
		MonoMethod* prinInt = mono_class_get_method_from_name(monoClass, "PrintInt", 1);
		int val = 5;
		void* params[1] = {
			&val
		};
		mono_runtime_invoke(prinInt, instance, params, nullptr);

		
		MonoMethod* prinInts = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
		int val1 = 15;
		int val2 = 51;
		void* params2[2] = {
			&val1,	&val2
		};
		mono_runtime_invoke(prinInts, instance, params2, nullptr);

		MonoString* monoStr = mono_string_new(s_Data->AppDomain, "Hello World from C++");
		MonoMethod* printCustomMessage = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		
		void* strParam = monoStr;
		mono_runtime_invoke(printCustomMessage, instance, &strParam, nullptr);
	}

	void ScriptingEngine::ShutdownMono()
	{
		// mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		// mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

}
