﻿#pragma once

#include "Mirage/Renderer/EditorCamera.h"
#include <enTT.hpp>

class b2World;

namespace Mirage
{
    class SceneObject;
    class TransformComponent;
    struct Relationship;
    class Scene
    {
    public:
        Scene() = default;
        ~Scene();

        SceneObject CreateSceneObject(const std::string& name = std::string());
        SceneObject CreateChildSceneObject(entt::entity parent, const std::string& name = std::string());
        void DestroySceneObject(SceneObject& entity);

		void OnRuntimeStart();
    	void OnRuntimeStop();

        void OnUpdateRuntime(float DeltaTime);
        void OnUpdateEditor(float DeltaTime, EditorCamera& camera);
        void OnViewportResize(uint32_t width, uint32_t height);

        SceneObject GetMainCameraSO();
        SceneObject GetSceneObject(entt::entity entity);

		float GetGravity() const { return m_Gravity; }
    	void SetGravity(float gravity) { m_Gravity = gravity; }
    	
    private:
        template <typename T>
        void OnComponentAdded(SceneObject& entity, T& component);
        
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0; uint32_t m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		float m_Gravity = -9.78f;

    	
        std::unordered_map<entt::entity, Relationship> m_Hierarchy;
        
        friend class SceneSerializer;
        friend class SceneObject;
        friend class TransformComponent;
        friend class HierarchyPanel;
    };
}
