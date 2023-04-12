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
    	auto& h = GetComponent<HierarchyComponent>();
		// reverse iterate children
    	for (auto it = h.m_Children.rbegin(); it != h.m_Children.rend(); ++it)
    	{
    		SceneObject so { *it, m_Scene };
    		so.Destroy();
    	}
    	
		h.m_Children.clear();
    	h.m_ChildrenSet.clear();

        if (HasParent())
        {
			auto parent = GetParent();
			auto& parentH = parent.GetComponent<HierarchyComponent>();
        	parentH.m_ChildrenSet.erase(h);
        	
        	for (uint32_t i = h.m_Index; i < parentH.m_Children.size() - 1; ++i)
			{
        		SceneObject so { parentH.m_Children[i + 1], m_Scene };
        		auto& childH = so.GetComponent<HierarchyComponent>();
				parentH.m_Children[i] = parentH.m_Children[i + 1];
        		parentH.m_ChildrenSet.erase(childH);
        		childH.m_Index--;
        		parentH.m_ChildrenSet.insert(childH);
			}
        	parentH.m_Children.pop_back();
        }
        else
        {
        	m_Scene->m_Hierarchy.erase(h.m_Index);
        	for (uint32_t i = h.m_Index; i < m_Scene->m_Hierarchy.size(); ++i)
        	{
        		SceneObject so { m_Scene->m_Hierarchy[i + 1].m_entity, m_Scene };
        		auto& childH = so.GetComponent<HierarchyComponent>();
        		m_Scene->m_Hierarchy.erase(childH.m_Index);
        		childH.m_Index--;
        		m_Scene->m_Hierarchy[childH.m_Index] = childH;
        	}
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
		UnParent();
	 
		m_Scene->m_Hierarchy.erase(GetComponent<HierarchyComponent>().m_Index);
	    
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
	    	HierarchyComponent& parentH = parentSo.GetComponent<HierarchyComponent>();
	    	HierarchyComponent& h = GetComponent<HierarchyComponent>();
	    	
	    	parentH.m_ChildrenSet.erase(h);
	    	for (uint32_t i = h.m_Index; i < parentH.m_Children.size() - 1; ++i)
	    	{
	    		SceneObject so { parentH.m_Children[i + 1], m_Scene };
	    		auto& childH = so.GetComponent<HierarchyComponent>();
	    		parentH.m_Children[i] = parentH.m_Children[i + 1];
	    		parentH.m_ChildrenSet.erase(childH);
	    		childH.m_Index--;
	    		parentH.m_ChildrenSet.insert(childH);
	    	}
	    	
	    	h.m_Parent = entt::null;
    
	    	h.m_Index = m_Scene->m_Hierarchy.size();
	    	m_Scene->m_Hierarchy[h.m_Index] = h;
	    }
    }
}
