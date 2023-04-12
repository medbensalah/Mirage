#pragma once

#include "Mirage/Renderer/EditorCamera.h"
#include <enTT.hpp>

#include "Mirage/Core/Time.h"
#include "Mirage/Core/UUID.h"

class b2World;

namespace Mirage
{
	class SceneObject;
    struct TransformComponent;
	struct HierarchyComponent;
	
    class Scene
    {
    public:
        Scene();
        ~Scene();

    	static Ref<Scene> Copy(const Ref<Scene> source);

        SceneObject CreateSceneObjectWithUUID(UUID uuid, const std::string& name = std::string());
        SceneObject CreateSceneObject(const std::string& name = std::string());
        SceneObject CreateChildSceneObjectWithUUID(UUID uuid, entt::entity parent, const std::string& name = std::string());
        SceneObject CreateChildSceneObject(entt::entity parent, const std::string& name = std::string());
        void DestroySceneObject(SceneObject& entity);

		void OnRuntimeStart();
    	void OnRuntimeStop();
		void OnSimulationStart();
		void OnSimulationStop();

        void OnUpdateRuntime(float DeltaTime);
        void OnUpdateSimulation(float DeltaTime, EditorCamera& camera);
        void OnUpdateEditor(float DeltaTime, EditorCamera& camera);

		void RenderRuntime();
    	
        void OnViewportResize(uint32_t width, uint32_t height);

    	SceneObject DuplicateSceneObject(SceneObject entity);

        SceneObject GetMainCameraSO();
        SceneObject GetSceneObject(entt::entity entity);
        SceneObject GetSceneObjectByUUID(UUID uuid);

    	template<typename... Components>
    	auto GetSceneObjectsWith()
    	{
    		return m_Registry.view<Components...>();
    	}
    
    private:
        template <typename T>
        void OnComponentAdded(SceneObject& entity, T& component);

    	void OnPhysics2DInit();
    	void OnPhysics2DStop();
    	
    	void RenderScene(EditorCamera& camera);
    	
    private:
        entt::registry m_Registry;
    	std::map<uint32_t, HierarchyComponent> m_Hierarchy;

    	uint32_t m_ViewportWidth = 0; uint32_t m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;
    	Timer m_PhysicsTimer;
    	
    	std::unordered_map<UUID, entt::entity> m_entt_uuid_map;
    	
        // std::unordered_map<entt::entity, Relationship> m_Hierarchy;
        
        friend class SceneSerializer;
        friend class SceneObject;
        friend struct TransformComponent;
        friend struct HierarchyComponent;
        friend class HierarchyPanel;
    };
}
