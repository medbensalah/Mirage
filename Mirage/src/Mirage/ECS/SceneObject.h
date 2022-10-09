#pragma once

#include "Scene.h"
#include <enTT.hpp>

namespace Mirage
{
    class SceneObject
    {
    public:
        SceneObject() = default;
        SceneObject(entt::entity entity, Scene* scene, const std::string& name = "SceneObject");
        SceneObject(const SceneObject& other) = default;

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            MRG_CORE_ASSERT(!HasComponent<T>(), "Entity already has that component!");
            return m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
        }

        template <typename T>
        T& GetComponent()
        {
            MRG_CORE_ASSERT(HasComponent<T>(), "Entity does not have that component!");
            return m_Scene->m_Registry.get<T>(m_Entity);
        }
        template <typename T>
        T& TryGetComponent()
        {
            return m_Scene->m_Registry.try_get<T>(m_Entity);
        }

        template <typename T>
        bool RemoveComponent()
        {
            MRG_CORE_ASSERT(HasComponent<T>(), "Entity does not have that component!");
            return m_Scene->m_Registry.remove<T>(m_Entity);
        }

        template <typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.any_of<T>(m_Entity);
        }

        operator bool() const { return m_Entity != entt::null; }
        
    public:
        const std::string& Name() { return m_name; }
        
    private:
        std::string m_name = "SceneObject";
        entt::entity m_Entity{ entt::null };
        Scene* m_Scene = nullptr;
    };
}
