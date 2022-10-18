#pragma once

#include "Scene.h"
#include <enTT.hpp>

#include "Components/TagComponent.h"

namespace Mirage
{

    struct Relationship
    {
        entt::entity m_Parent = entt::null;
        std::vector<entt::entity> m_Children;
    };
    
    class SceneObject
    {
    public:        
        SceneObject() = default;
        SceneObject(entt::entity entity, Scene* scene);
        SceneObject(const SceneObject& other) = default;
                
        void Destroy();

        void AddChild(entt::entity child);
        void RemoveChild(entt::entity child);
        void SetParent(entt::entity parent);

        size_t GetChildCount()
        {
            return m_Scene->m_Hierarchy.at(m_Entity).m_Children.size();
        }
        SceneObject GetChild(size_t index)
        {
            return SceneObject(m_Scene->m_Hierarchy.at(m_Entity).m_Children[index], m_Scene);
        }
        SceneObject GetParent()
        {
            return SceneObject(m_Scene->m_Hierarchy.at(m_Entity).m_Parent, m_Scene);
        }
        bool HasParent()
        {
            MRG_CORE_WARN("{0} {1}", GetComponent<TagComponent>().Tag, (uint32_t)m_Entity);
            return m_Scene->m_Hierarchy.at(m_Entity).m_Parent != entt::null;
        }
        std::vector<entt::entity> GetChildren()
        {
            return m_Scene->m_Hierarchy.at(m_Entity).m_Children;
        }
        
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
        operator entt::entity() const { return m_Entity; }

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
        
        friend class HierarchyPanel;
    };
}
