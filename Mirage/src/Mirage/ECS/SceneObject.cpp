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

    void SceneObject::AddChild(entt::entity childe)
    {
    	SceneObject child = { childe, m_Scene };
    	child.SetParent(m_Entity);
    }

    void SceneObject::RemoveChild(entt::entity childe)
    {
    	SceneObject child = { childe, m_Scene };
    	child.UnParent();
    }

    void SceneObject::SetParent(entt::entity parent)
    {
	    if (HasParent())
	    {
		    UnParent();
	    }
	    else
	    {
		    m_Scene->m_Hierarchy.erase(GetComponent<HierarchyComponent>());
	    }
    	SceneObject parentSo = {parent, m_Scene};
    	HierarchyComponent& h = parentSo.GetComponent<HierarchyComponent>();
    	HierarchyComponent& childH = GetComponent<HierarchyComponent>();
    	
    	h.m_Children.push_back(m_Entity);
    	childH.index = h.m_Children.size() - 1;
    	h.m_ChildrenSet.insert(childH);
    	
        GetComponent<HierarchyComponent>().m_Parent = parent;
    }

    void SceneObject::UnParent()
    {
	    if (HasParent())
	    {
		    SceneObject parentSo = GetParent();
	    	HierarchyComponent& h = parentSo.GetComponent<HierarchyComponent>();
	    	HierarchyComponent& childH = GetComponent<HierarchyComponent>();

	    	auto v = &h.m_Children;
	    	auto it = std::find(v->begin(), v->end(), m_Entity);
	    	if (it != v->end())
	    	{
	    		v->erase(it);
	    	}
    
	    	childH.index = m_Scene->m_Hierarchy.size() - 1;
	    	m_Scene->m_Hierarchy.insert(childH);
    
	    	GetComponent<HierarchyComponent>().m_Parent = entt::null;
	    }
    }
}
