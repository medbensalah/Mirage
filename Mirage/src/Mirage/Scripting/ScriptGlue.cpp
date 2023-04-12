#include "MrgPch.h"

#include "ScriptGlue.h"

#include "ScriptingEngine.h"
#include "Mirage/Core/Input.h"
#include "Mirage/Core/UUID.h"
#include "Mirage/ECS/Scene.h"
#include "Mirage/ECS/SceneObject.h"
#include "Mirage/ECS/Components/Base/TransformComponent.h"
#include "Mirage/Math/glmTypes.h"
#include "mono/metadata/object.h"

namespace Mirage
{
#define MRG_ADD_INTERRAL_CALL(Name) mono_add_internal_call("Mirage.InternalCalls::" #Name, Name)
	
	static void CppLogFunc(MonoString* CSstr)
	{
		char* cstr = mono_string_to_utf8(CSstr);
		std::string str(cstr);
		mono_free(cstr);
		MRG_CORE_INFO("C#: {}", str);
	}
	
	static void SO_GetPosition(UUID uuid, Vec3* outVec)
	{
		Scene* scene =ScriptingEngine::GetSceneContext();
		SceneObject so = scene->GetSceneObjectByUUID(uuid);
		*outVec = so.GetComponent<TransformComponent>().Position();
	}
	static void SO_SetPosition(UUID uuid, Vec3* outVec)
	{
		Scene* scene =ScriptingEngine::GetSceneContext();
		SceneObject so = scene->GetSceneObjectByUUID(uuid);
		so.GetComponent<TransformComponent>().SetPosition(*outVec);
	}
	static bool Input_IsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}
	

	void ScriptGlue::RegisterFunctions()
	{
		MRG_ADD_INTERRAL_CALL(CppLogFunc);
		MRG_ADD_INTERRAL_CALL(SO_GetPosition);
		MRG_ADD_INTERRAL_CALL(SO_SetPosition);
		MRG_ADD_INTERRAL_CALL(Input_IsKeyDown);
	}
}
