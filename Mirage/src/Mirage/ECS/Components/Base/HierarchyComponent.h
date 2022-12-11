#pragma once

#include <enTT.hpp>

namespace Mirage
{
	class Scene;
	
	struct HierarchyComponent
	{
		HierarchyComponent() = default;
		HierarchyComponent(const HierarchyComponent&) = default;

		entt::entity m_Parent = entt::null;
		entt::entity m_entity = entt::null;
		std::vector<entt::entity> m_Children;
		uint32_t index;

		// operator <
		bool operator<(const HierarchyComponent& other) const
		{
			return index < other.index;
		}
		
		std::set<HierarchyComponent> m_ChildrenSet;
	};
}
