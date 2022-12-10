#include <MrgPch.h>
#include "SceneObject.h"

#include "Components/Base/TagComponent.h"
#include "Components/Base/TransformComponent.h"

namespace Mirage
{
    SceneObject::SceneObject(entt::entity entity, Scene* scene)
        : m_Entity(entity), m_Scene(scene)
    {
    }

    void SceneObject::Destroy()
    {
        if(HasParent())
        {
            GetParent().RemoveChild(m_Entity);
        }
        auto v =GetComponent<HierarchyComponent>().m_Children;
        for(auto child : v)
        {
            SceneObject so = {child, m_Scene};
            so.Destroy();
        }
        m_Scene->m_Registry.destroy(m_Entity);
    }

    void SceneObject::AddChild(entt::entity entity)
    {
        GetComponent<HierarchyComponent>().m_Children.push_back(entity);
        SceneObject so = {entity, m_Scene};
        so.SetParent(m_Entity);
    }

    void SceneObject::RemoveChild(entt::entity child)
    {
        auto v = &GetComponent<HierarchyComponent>().m_Children;
        auto it = std::find(v->begin(), v->end(), child);
        if (it != v->end())
        {
            v->erase(it);
        }
        SceneObject so = {child, m_Scene};
        so.RemoveParent();
    }

    void SceneObject::SetParent(entt::entity parent)
    {
        GetComponent<HierarchyComponent>().m_Parent = parent;
    }

    void SceneObject::RemoveParent()
    {
        GetComponent<HierarchyComponent>().m_Parent = entt::null;
    }
}
