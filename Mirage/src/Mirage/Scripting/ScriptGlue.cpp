#include "MrgPch.h"

#include "ScriptGlue.h"

#include "ScriptingEngine.h"
#include "box2d/b2_body.h"
#include "Mirage/Core/Input.h"
#include "Mirage/Core/UUID.h"
#include "Mirage/ECS/Scene.h"
#include "Mirage/ECS/SceneObject.h"
#include "Mirage/ECS/Components/AllComponents.h"
#include "Mirage/Math/glmTypes.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

namespace Mirage
{

	
#define MRG_ADD_INTERRAL_CALL(Name) mono_add_internal_call("Mirage.Internals::" #Name, Name)
	
	static std::unordered_map<MonoType*, std::function<bool(SceneObject)>> m_HasComponentFuncs;
	static void CppLogFunc(MonoString* CSstr)
	{
		char* cstr = mono_string_to_utf8(CSstr);
		std::string str(cstr);
		mono_free(cstr);
		MRG_CORE_INFO("C#: {}", str);
	}
	
	static bool SO_HasComponent(UUID uuid, MonoReflectionType* type)
	{
		Scene* scene =ScriptingEngine::GetSceneContext();
		MRG_CORE_ASSERT(scene, "Scene is null");
		SceneObject so = scene->GetSceneObjectByUUID(uuid);
		MRG_CORE_ASSERT(so, "SceneObject is null");

		MonoType* monoType = mono_reflection_type_get_type(type);
		MRG_CORE_ASSERT(m_HasComponentFuncs.find(monoType) != m_HasComponentFuncs.end(), "Component not registered");
		return m_HasComponentFuncs[monoType](so);
	}
	
	static void Transform_GetPosition(UUID uuid, Vec3* outVec)
	{
		Scene* scene =ScriptingEngine::GetSceneContext();
		MRG_CORE_ASSERT(scene, "Scene is null");
		SceneObject so = scene->GetSceneObjectByUUID(uuid);
		MRG_CORE_ASSERT(so, "SceneObject is null");
		
		*outVec = so.GetComponent<TransformComponent>().Position();
	}
	static void Transform_SetPosition(UUID uuid, Vec3* outVec)
	{
		Scene* scene =ScriptingEngine::GetSceneContext();
		MRG_CORE_ASSERT(scene, "Scene is null");
		SceneObject so = scene->GetSceneObjectByUUID(uuid);
		MRG_CORE_ASSERT(so, "SceneObject is null");
		
		so.GetComponent<TransformComponent>().SetPosition(*outVec);
	}

	static void Rigidbody2D_ApplyLinearImpulse(UUID uuid, Vec2* impulse, Vec2* point, bool wake)
	{
		Scene* scene =ScriptingEngine::GetSceneContext();
		MRG_CORE_ASSERT(scene, "Scene is null");
		SceneObject so = scene->GetSceneObjectByUUID(uuid);
		MRG_CORE_ASSERT(so, "SceneObject is null");

		auto& rb2d = so.GetComponent<Rigidbody2DComponent>();
		b2Body* body = rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}
	
	static void Rigidbody2D_ApplyLinearImpulseToCenter(UUID uuid, Vec2* impulse, Vec2* point, bool wake)
	{
		Scene* scene =ScriptingEngine::GetSceneContext();
		MRG_CORE_ASSERT(scene, "Scene is null");
		SceneObject so = scene->GetSceneObjectByUUID(uuid);
		MRG_CORE_ASSERT(so, "SceneObject is null");

		auto& rb2d = so.GetComponent<Rigidbody2DComponent>();
		b2Body* body = rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}
	
	
	static bool Input_IsKeyDown(KeyCode keyCode)
	{
		return Input::IsKeyPressed(keyCode);
	}

	template <typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			//remove the word component
			std::string managedName = fmt::format("Mirage.{}", typeName.substr(pos + 1, typeName.size() - pos - 10));
		
			MonoType* managedType = mono_reflection_type_from_name(managedName.data(), ScriptingEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				MRG_CORE_ERROR("Component {} not found in assembly", managedName);
				return;
			}
			m_HasComponentFuncs[managedType] =  [](SceneObject so){ return so.HasComponent<Component>(); };
		}(), ...);
	}

	template <typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		MRG_ADD_INTERRAL_CALL(Input_IsKeyDown);
		// MRG_ADD_INTERRAL_CALL(CppLogFunc);
		
		MRG_ADD_INTERRAL_CALL(SO_HasComponent);
		MRG_ADD_INTERRAL_CALL(Transform_GetPosition);
		MRG_ADD_INTERRAL_CALL(Transform_SetPosition);
		
		MRG_ADD_INTERRAL_CALL(Rigidbody2D_ApplyLinearImpulse);
		MRG_ADD_INTERRAL_CALL(Rigidbody2D_ApplyLinearImpulseToCenter);
	}
}
