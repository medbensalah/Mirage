#pragma once
#include <vector>

#include "VisualComponent.h"
#include "Mirage/Core/Core.h"

namespace Mirage::VisualComponents
{
	class Container : public VisualComponent
	{
	public:
		Container() {}
		virtual ~Container() override
		{
			m_Components.clear();
		}
		void Draw(float scale) override;

		virtual Ref<VisualComponent> Add(VisualComponent* component);
		virtual Ref<VisualComponent> Add(Ref<VisualComponent> component);
		
		void Remove(Ref<VisualComponent> component);

		void Clear();

		void SetSize(const ImVec2& size) { m_Size = size; }
		ImVec2 GetSize() const { return m_Size; }
		
	protected:
		std::vector<Ref<VisualComponent>> m_Components;

	private:
		ImVec2 m_Size;
	};
}
