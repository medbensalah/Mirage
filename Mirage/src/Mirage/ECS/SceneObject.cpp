﻿#include <MrgPch.h>
#include "SceneObject.h"

#include "Components/TagComponent.h"
#include "Components/TransformComponent.h"

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
        auto v = &m_Scene->m_Hierarchy.at(m_Entity).m_Children;
        for(auto child : *v)
        {
            SceneObject so = {child, m_Scene};
            so.Destroy();
        }
        m_Scene->m_Registry.destroy(m_Entity);
    }

    void SceneObject::AddChild(entt::entity entity)
    {
        m_Scene->m_Hierarchy.at(m_Entity).m_Children.push_back(entity);
        SceneObject so = {entity, m_Scene};
        m_Scene->m_Hierarchy.at(so).m_Parent = m_Entity;
    }

    void SceneObject::RemoveChild(entt::entity child)
    {
        auto v = &m_Scene->m_Hierarchy.at(m_Entity).m_Children;
        auto it = std::find(v->begin(), v->end(), child);
        if (it != v->end())
        {
            v->erase(it);
        }
        SceneObject so = {child, m_Scene};
        so.m_Scene->m_Hierarchy.at(m_Entity).m_Parent = entt::null;
    }

    void SceneObject::SetParent(entt::entity parent)
    {
        m_Scene->m_Hierarchy.at(m_Entity).m_Parent = parent;
    }
}
