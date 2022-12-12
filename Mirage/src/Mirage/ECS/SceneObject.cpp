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
        	UnParent();
        }

    	auto& h = GetComponent<HierarchyComponent>();
    	
        auto children = h.m_Children;
        for(auto child : children)
        {
            SceneObject so = {child, m_Scene};
            so.Destroy();
        }

    	m_Scene->m_Hierarchy.erase(h);
    	
    	auto childIt = m_Scene->m_Hierarchy.find(h);
    	for (auto it = childIt; it != m_Scene->m_Hierarchy.end(); ++it)
    	{
    		SceneObject so = { it->m_entity, m_Scene };
    		auto& hc = so.GetComponent<HierarchyComponent>();
    		hc.m_Index--;
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
    	childH.m_Index = h.m_Children.size() - 1;
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

	    	auto it = std::find(h.m_Children.begin(), h.m_Children.end(), m_Entity);
	    	if (it != h.m_Children.end())
	    	{
	    		h.m_Children.erase(it);
	    	}
	    	uint32_t childIndex = childH.m_Index;
	    	
	    	h.m_ChildrenSet.erase(childH);
	    	auto childIt = h.m_ChildrenSet.find(childH);
	    	for (auto it = childIt; it != h.m_ChildrenSet.end(); ++it)
	    	{
	    		SceneObject so = { it->m_entity, m_Scene };
	    		auto& hc = so.GetComponent<HierarchyComponent>();
	    		hc.m_Index--;
	    	}
	    	
	    	childH.m_Parent = entt::null;
    
	    	childH.m_Index = m_Scene->m_Hierarchy.size();
	    	m_Scene->m_Hierarchy.insert(childH);
	    }
    }
}
