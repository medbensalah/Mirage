#include "Node.h"

#include "Mirage/Definitions/Textures.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"
#include "Mirage/ImGui/Extensions/GraphStyle.h"
#include "Mirage/Renderer/Texture.h"
#include "VisualComponents/Text.h"

namespace Mirage::Graph
{
	static int i = 0;
	Ref<Texture2D> Node::bg;
	Node::Node(std::string title, ImVec2 position)
		: m_Title(title), m_Position(position)
	{
		if (!i)
		{
			++i;
			bg = Texture2D::Create(Textures::NodeHeaderBackground);
		}
		// m_VisualComponents.push_back(new VisualComponents::Text(title));
		
		m_VisualComponents.push_back(new VisualComponents::Text(m_Title,
			ImColor{ 255, 255, 255, 255 },
	0.1f, 0.01f, 18.0f));
	}

	static bool o = true;
	void Node::OnImGuiRender(ImVec2 origin, float zoom)
	{
		m_Zoom = zoom;
		ImVec2 nodePosition = m_Position * zoom + origin;

		ImGui::SetNextWindowPos(nodePosition);
		ImGui::BeginChild(m_Title.c_str(), m_Size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::SetWindowFontScale(zoom);

		
		ImGui::BeginGroup();
		DrawHeader();
		Draw();
		ImGui::EndGroup();

		DrawBorder();
		
		UpdateCoordinates();
		
		ImGui::EndChild();
		
	}

	void Node::DrawHeader()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 uv0 = ImVec2(0, 0.25f);
		ImVec2 uv1 = ImVec2(1, 0.5f);
		
		drawList->AddImageRounded((ImTextureID) bg->GetRendererID(),
			ImGui::GetCursorScreenPos(),
			ImGui::GetCursorScreenPos() + ImVec2(ImGui::GetWindowWidth(), 20 * m_Zoom),
			uv0, uv1,
			m_HeaderColor, ImGui::GetStyle().ChildRounding, ImDrawCornerFlags_Top);
	}

	void Node::Draw()
	{
		for (auto& visualComponent : m_VisualComponents)
		{
			visualComponent->Draw(m_Zoom);
		}
	}

	void Node::DrawBorder()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRect(ImGui::GetWindowPos(),
		                  ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMax(),
		                  Style::NodeBorderColor, ImGui::GetStyle().ChildRounding, ImDrawCornerFlags_All,
		                  m_Zoom);
	}

	
	void Node::UpdateCoordinates()
	{
		m_Size = ImGui::GetItemRectSize() + 35.0f * m_Zoom;
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(0))
		{
			m_Position = m_Position + ImGui::GetMouseDragDelta() / m_Zoom;
			ImGui::ResetMouseDragDelta();
		}
	}
}
