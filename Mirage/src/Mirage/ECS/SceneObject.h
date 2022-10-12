#pragma once

#include "Scene.h"
#include <enTT.hpp>

namespace Mirage
{
    class SceneObject
    {
    public:
        SceneObject() = default;
        SceneObject(entt::entity entity, Scene* scene);
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
        operator uint32_t() const { return (uint32_t)m_Entity; }

        bool operator== (const SceneObject& other) const
        {
            return m_Entity == other.m_Entity && m_Scene == other.m_Scene;
        }
        bool operator!= (const SceneObject& other) const
        {
            return !(*this == other);
        }
    private:
        entt::entity m_Entity{ entt::null };
        Scene* m_Scene = nullptr;
    };
}
