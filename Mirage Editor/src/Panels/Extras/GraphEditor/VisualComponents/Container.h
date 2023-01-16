#pragma once
#include <vector>

#include "VisualComponent.h"
#include "ImGui/imgui.h"

namespace Mirage::VisualComponents
{
	class Container : public VisualComponent
	{
	public:
		Container() {}
		virtual ~Container() override {}
		void Draw(float scale) override;

		void Add(VisualComponent* component);
		void AddComponents(std::vector<VisualComponent*> components);
		
		void Remove(VisualComponent* component);

		void Clear();

		void SetSize(const ImVec2& size) { m_Size = size; }
		ImVec2 GetSize() const { return m_Size; }
		
	private:
		std::vector<VisualComponent*> m_Components;

		ImVec2 m_Size;
	};
}
