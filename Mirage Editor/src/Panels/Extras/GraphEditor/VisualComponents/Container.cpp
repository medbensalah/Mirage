#include "Container.h"

namespace Mirage::VisualComponents
{
	void Container::Draw(float scale)
	{
		for (auto& component : m_Components)
		{
			component->Draw(scale);
		}
	}

	void Container::Add(VisualComponent* component)
	{
		m_Components.push_back(component);
	}

	void Container::AddComponents(std::vector<VisualComponent*> components)
	{
		m_Components.insert(m_Components.end(), components.begin(), components.end());
	}

	void Container::Remove(VisualComponent* component)
	{
		m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), component), m_Components.end());
	}

	void Container::Clear()
	{
		m_Components.clear();
	}
}
