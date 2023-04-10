#include "MrgPch.h"

#include "ScriptGlue.h"

#include "Mirage/Math/glmTypes.h"
#include "mono/metadata/object.h"

namespace Mirage
{
#define MRG_ADD_INTERRAL_CALL(Name) mono_add_internal_call("Mirage.InternalCalls::" #Name, Name)
	
	static void CppLogFunc(MonoString* CSstr, int val)
	{
		char* cstr = mono_string_to_utf8(CSstr);
		std::string str(cstr);
		mono_free(cstr);
		MRG_CORE_INFO("C++ logging from C# call: {} {}", str, val);
	}

	static void CppLogFuncVec3(Vec3* vec, Vec3* outVal)
	{
		MRG_CORE_INFO("C++ logging from C# call vec: {}", *vec);
		// *outVal = glm::cross(*vec, Vec3(vec->x, vec->y, -vec->z));
	}

	void ScriptGlue::RegisterFunctions()
	{
		MRG_ADD_INTERRAL_CALL(CppLogFunc);
		MRG_ADD_INTERRAL_CALL(CppLogFuncVec3);
	}
}
