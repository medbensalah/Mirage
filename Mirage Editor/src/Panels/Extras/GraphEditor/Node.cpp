#include "Node.h"

#include "glm/ext/scalar_common.hpp"
#include "Mirage/Definitions/Textures.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"
#include "Mirage/ImGui/Extensions/GraphStyle.h"
#include "Mirage/Renderer/Texture.h"
#include "VisualComponents/Text.h"

namespace Mirage::Graph
{
	using namespace VisualComponents;
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
		CustomContentSize = InputContainerSize = OutputContainerSize = HeaderSize = m_Size = ImVec2(0, 0);

		HeaderContainer.Add(new Text(m_Title, ImColor(255, 255, 255, 255), 22.0f, true));
		// InputContainer.Add(new Text("Input"));
		InputContainer.Add(new Text("I"));

		OutputContainer.Add(new Text("Output"));
		OutputContainer.Add(new Text("Output"));
		OutputContainer.Add(new Text("Output"));
		OutputContainer.Add(new Text("Output"));
	}

	static bool o = true;
	void Node::OnImGuiRender(ImVec2 origin, float zoom)
	{
		m_Zoom = zoom;
		ImVec2 nodePosition = m_Position * zoom + origin;

		ImGui::SetNextWindowPos(nodePosition);
		if(ImGui::BeginChild(m_Title.c_str(), m_Size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImGui::SetWindowFontScale(zoom);
		
			ImGui::BeginGroup();
			DrawHeader();
			DrawInputContainer();
			DrawOutputContainer();
			DrawCustomContent();
			ImGui::EndGroup();
			UpdateSize();
		
			DrawBorder();
			UpdateCoordinates();
		}
		ImGui::EndChild();

	}

	void Node::DrawHeader()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 uv0 = ImVec2(0, 0.25f);
		ImVec2 uv1 = ImVec2(1, 0.5f);
		
		drawList->AddImageRounded((ImTextureID) bg->GetRendererID(),
			ImGui::GetCursorScreenPos(),
			ImGui::GetCursorScreenPos() + HeaderSize,
			uv0, uv1,
			m_HeaderColor, ImGui::GetStyle().ChildRounding, ImDrawCornerFlags_Top);
		
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f * m_Zoom);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7.0f * m_Zoom);
		HeaderContainer.Draw(m_Zoom);
		HeaderSize = ImVec2(ImGui::GetItemRectSize().x + 20.0f * m_Zoom, 35 * m_Zoom);
	}

	void Node::DrawInputContainer()
	{
		ImGui::SetCursorPosY(HeaderSize.y);
		ImGui::BeginGroup();
		ImGui::Indent();
		InputContainer.Draw(m_Zoom);
		ImGui::Unindent();
		ImGui::EndGroup();
		ImVec2 containerMin = ImGui::GetItemRectMin();
		ImVec2 containerMax = ImGui::GetItemRectMax();
		InputContainerSize = containerMax - containerMin;
		// ImGui::GetWindowDrawList()->AddRectFilled(containerMin, containerMax, IM_COL32(0, 255, 0, 16),0,0);
	}

	void Node::DrawOutputContainer()
	{
		ImGui::SetCursorPosY(HeaderSize.y);
		ImGui::SetCursorPosX(m_Size.x - OutputContainerSize.x);
		ImGui::BeginGroup();
		OutputContainer.Draw(m_Zoom);
		ImGui::EndGroup();
		ImVec2 containerMin = ImGui::GetItemRectMin();
		ImVec2 containerMax = ImGui::GetItemRectMax();
		OutputContainerSize = containerMax - containerMin;
		// ImGui::GetWindowDrawList()->AddRectFilled(containerMin, containerMax, IM_COL32(255, 0, 0, 16),0,0);
	
	}

	void Node::DrawCustomContent()
	{
		CustomContentContainer.Draw(m_Zoom);
	}

	void Node::DrawBorder()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		if (ImGui::IsWindowFocused())
		{
			drawList->AddRect(ImGui::GetWindowPos(),
							  ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMax(),
							  Style::NodeBorderColorFocused, ImGui::GetStyle().ChildRounding, ImDrawCornerFlags_All,
							  3);
		}
		else if (ImGui::IsWindowHovered())
		{
			drawList->AddRect(ImGui::GetWindowPos(),
							  ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMax(),
							  Style::NodeBorderColorHovered, ImGui::GetStyle().ChildRounding, ImDrawCornerFlags_All,
							  3);
		}
		drawList->AddRect(ImGui::GetWindowPos(),
						  ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMax(),
						  Style::NodeBorderColor, ImGui::GetStyle().ChildRounding, ImDrawCornerFlags_All,
						  1);			
		
	}
	
	void Node::UpdateCoordinates()
	{
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(0))
		{
			m_Position = m_Position + ImGui::GetMouseDragDelta() / m_Zoom;
			ImGui::ResetMouseDragDelta();
		}
	}

	void Node::UpdateSize()
	{
		HeaderSize.x = std::max(glm::max(HeaderSize.x, CustomContentSize.x), glm::max(InputContainerSize.x, OutputContainerSize.x) * 2.0f);
		m_Size.x = HeaderSize.x;
		m_Size.y = HeaderSize.y + CustomContentSize.y + glm::max(InputContainerSize.y, OutputContainerSize.y);
		HeaderContainer.SetSize(HeaderSize);
		CustomContentContainer.SetSize(CustomContentSize);
		InputContainer.SetSize(InputContainerSize);
		OutputContainer.SetSize(OutputContainerSize);
	}
}
