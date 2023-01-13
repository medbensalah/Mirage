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

	static bool o = true;
	void Node::Draw(ImVec2 offset, float zoom)
	{
		m_Scale = zoom;
		ImVec2 nodePosition = (m_Position + m_Drag) * zoom + offset;
		ImGui::SetNextWindowPos(nodePosition, ImGuiCond_Always);
		ImGui::BeginChild(m_Title.c_str(), m_Size, true, ImGuiWindowFlags_NoScrollbar |
		                  ImGuiWindowFlags_NoScrollWithMouse);
		
		ImGui::SetWindowFontScale(zoom);
		
		ImGui::BeginGroup();
		for (auto& visualComponent : m_VisualComponents)
		{
			visualComponent->Draw(m_Scale);
		}
		ImGui::EndGroup();
		Drag();
		
		ImGui::EndChild();
	}

	void Node::Drag()
	{
		m_Size = ImGui::GetItemRectSize() + 35.0f * m_Scale;
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(0))
		{
			m_Drag = m_Drag + ImGui::GetMouseDragDelta() / m_Scale;
			ImGui::ResetMouseDragDelta();
		}
	}
}
