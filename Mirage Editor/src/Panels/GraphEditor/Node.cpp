#include "Node.h"

#include "Mirage/ImGui/Extensions/DrawingAPI.h"
#include "VisualComponents/Text.h"

namespace Mirage::Graph
{
	Node::Node(std::string title, ImVec2 position)
		: m_Title(title), m_Position(position)
	{
		m_VisualComponents.push_back(new VisualComponents::Text(title));
	}

	void Node::Draw(ImVec2 offset, float zoom)
	{
		m_Scale = zoom;
		ImVec2 nodePosition = m_Position + offset;
		ImGui::SetCursorScreenPos(nodePosition);
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->AddRectFilled(nodePosition, nodePosition + ImVec2(100, 100) * zoom, IM_COL32(255, 255, 255, 255));
		
		for (auto& visualComponent : m_VisualComponents)
		{
			visualComponent->Draw(m_Scale);
		}
	}
}
