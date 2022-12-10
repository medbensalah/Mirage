#pragma once

#include "Mirage/Core/UUID.h"
#include "Mirage/ECS/Components/Base/IDCompoennt.h"
#include "Scene.h"

#include <enTT.hpp>

#include "Components/Base/HierarchyComponent.h"

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

        Scene* GetScene() const { return m_Scene; }
        uint32_t GetEntity() const { return (uint32_t)m_Entity; }
        
        size_t GetChildCount()
        {
            return GetComponent<HierarchyComponent>().m_Children.size();
        }
        SceneObject GetChild(size_t index)
        {
            return SceneObject(GetComponent<HierarchyComponent>().m_Children[index], m_Scene);
        }
        SceneObject GetParent()
        {
            return SceneObject(GetComponent<HierarchyComponent>().m_Parent, m_Scene);
        }
        bool HasParent()
        {
            return GetComponent<HierarchyComponent>().m_Parent != entt::null;
        }
        std::vector<entt::entity> GetChildren()
        {
            return GetComponent<HierarchyComponent>().m_Children;
        }
        
        template <typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            MRG_CORE_ASSERT(!HasComponent<T>(), "Entity already has that component!");
            T& component = m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
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

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
    	
        bool operator== (const SceneObject& other) const
        {
            return m_Entity == other.m_Entity && m_Scene == other.m_Scene;
        }
        bool operator!= (const SceneObject& other) const
        {
            return !(*this == other);
        }

    private:
        void AddChild(entt::entity child);
        void RemoveChild(entt::entity child);
        void SetParent(entt::entity parent);
        void RemoveParent();
        
        entt::entity m_Entity{ entt::null };
        Scene* m_Scene = nullptr;
        
        friend class Scene;
        friend class HierarchyPanel;
    };
}
