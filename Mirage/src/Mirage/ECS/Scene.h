﻿#pragma once

#include "Mirage/Renderer/EditorCamera.h"
#include <enTT.hpp>

namespace Mirage
{
    class SceneObject;
    class TransformComponent;
    struct Relationship;
    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        SceneObject CreateSceneObject(const std::string& name = std::string());
        SceneObject CreateChildSceneObject(entt::entity parent, const std::string& name = std::string());
        void DestroySceneObject(SceneObject& entity);

        void OnUpdateRuntime(float DeltaTime);
        void OnUpdateEditor(float DeltaTime, EditorCamera& camera);
        void OnViewportResize(uint32_t width, uint32_t height);

        SceneObject GetMainCameraSO();
        SceneObject GetSceneObject(entt::entity entity);

    private:
        template <typename T>
        void OnComponentAdded(SceneObject& entity, T& component);
        
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0; uint32_t m_ViewportHeight = 0;

        std::unordered_map<entt::entity, Relationship> m_Hierarchy;
        
        friend class SceneSerializer;
        friend class SceneObject;
        friend class TransformComponent;
        friend class HierarchyPanel;
    };
}
