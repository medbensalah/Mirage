#pragma once

#include <enTT.hpp>

namespace Mirage
{
	class Scene;
	
	struct HierarchyComponent
	{
		HierarchyComponent() = default;
		HierarchyComponent(const HierarchyComponent&) = default;

		void SetIndex(uint32_t index) { m_Index = index; }
		
		entt::entity m_Parent = entt::null;
		entt::entity m_entity = entt::null;
		std::vector<entt::entity> m_Children;
		
		uint32_t m_Index;

		// operator <
		bool operator<(const HierarchyComponent& other) const
		{
			return m_Index < other.m_Index;
		}
		
		std::set<HierarchyComponent> m_ChildrenSet;
	};
}
