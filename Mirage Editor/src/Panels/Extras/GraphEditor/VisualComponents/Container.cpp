#include "Container.h"

#include "Mirage/Core/Log.h"

namespace Mirage::VisualComponents
{
	void Container::Draw(float scale)
	{
		for (auto& component : m_Components)
		{
			component->Draw(scale);
		}
	}

	Ref<VisualComponent> Container::Add(VisualComponent* component)
	{
		Ref<VisualComponent> componentPtr(component);
		m_Components.push_back(componentPtr);
		return componentPtr;
	}

	Ref<VisualComponent> Container::Add(Ref<VisualComponent> component)
	{
		m_Components.push_back(std::move(component));
		return component;
	}
	
	void Container::Remove(Ref<VisualComponent> component)
	{
		m_Components.erase
		(
			std::remove_if
			(
				m_Components.begin(),
				m_Components.end(),
				[& component](const Ref<VisualComponent>& ps) { return ps.get() == component.get(); }
			),
			m_Components.end()
		);
	}

	void Container::Clear()
	{
		m_Components.clear();
	}
}
