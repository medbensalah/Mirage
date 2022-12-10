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
		std::vector<entt::entity> m_Children = {};
	};
}
