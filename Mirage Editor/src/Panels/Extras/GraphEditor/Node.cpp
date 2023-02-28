#include "Node.h"

#include "glm/ext/scalar_common.hpp"
#include "Mirage/Definitions/Textures.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"
#include "Mirage/ImGui/Extensions/GraphStyle.h"
#include "Mirage/Renderer/Texture.h"
#include "VisualComponents/Port.h"

namespace Mirage::Graph
{
	using namespace VisualComponents;
	bool Node::Initialized = false;
	
	Ref<Texture2D> Node::bg;
	Node::Node(GraphEditor* graph, std::string title, ImVec2 position)
		: m_Graph(graph), m_Title(title), m_Position(position) 
	{
		if (!Initialized)
		{
			Initialized = true;
			bg = Texture2D::Create(Textures::NodeHeaderBackground);
		}
		CustomContentSize = InputContainerSize = OutputContainerSize = HeaderSize = m_Size = ImVec2(0, 0);

		HeaderContainer.Add(new Text(m_Title, ImColor(255, 255, 255, 255), 22.0f, true));

		// Add all port types
		// AddPort(PortDataType::Bool,		PortType::Input);
		// AddPort(PortDataType::Int,		PortType::Input);
		// AddPort(PortDataType::Int2, 	PortType::Input);
		// AddPort(PortDataType::Int3, 	PortType::Input);
		// AddPort(PortDataType::Int4, 	PortType::Input);
		// AddPort(PortDataType::Float, 	PortType::Input);
		// AddPort(PortDataType::Float2, 	PortType::Input);
		// AddPort(PortDataType::Float3, 	PortType::Input);
		// AddPort(PortDataType::Float4, 	PortType::Input);
		// AddPort(PortDataType::Color, 	PortType::Input);
		// AddPort(PortDataType::String,	PortType::Input);
		//
		// AddPort(PortDataType::Bool,		PortType::Output);
		// AddPort(PortDataType::Int,		PortType::Output);
		// AddPort(PortDataType::Int2, 	PortType::Output);
		// AddPort(PortDataType::Int3, 	PortType::Output);
		// AddPort(PortDataType::Int4, 	PortType::Output);
		// AddPort(PortDataType::Float, 	PortType::Output);
		// AddPort(PortDataType::Float2, 	PortType::Output);
		// AddPort(PortDataType::Float3, 	PortType::Output);
		// AddPort(PortDataType::Float4, 	PortType::Output);
		// AddPort(PortDataType::Color, 	PortType::Output);
		// AddPort(PortDataType::String,	PortType::Output);
		bool b = true;
		float f[4] = { 1.0f, 2.0f, 3.0f, 0.0f };

		Ref<Float4> f4 = CreateRef<Float4>(f);
		
		auto porti = AddPort(PortDataType::Float4,	PortType::Input, f4);
		auto porto = AddPort(PortDataType::Float4,	PortType::Output, f4);
		// InputContainer.Add(new Boolean(&b));
		// InputContainer.Add(new Float(f[0]));
		// InputContainer.Add(new Float2(f));
		// InputContainer.Add(new Float3(f));
		// InputContainer.Add(new Float4(f));
		// InputContainer.Add(new Color(&color));
	}

	void Node::OnImGuiRender(ImVec2 origin, float zoom)
	{
		float oldZoom = m_Zoom;
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
			UpdateCoordinates();
		
			DrawBorder();
		}
		ImGui::EndChild();
	}

	void Node::DrawHeader()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		
		drawList->AddImageRounded((ImTextureID) bg->GetRendererID(),
			ImGui::GetCursorScreenPos(),
			ImGui::GetCursorScreenPos() + HeaderSize,
			ImVec2(0, 0), ImVec2(1, 1),
			m_HeaderColor, ImGui::GetStyle().ChildRounding, ImDrawCornerFlags_Top);
		
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f * m_Zoom);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7.0f * m_Zoom);
		HeaderContainer.Draw(m_Zoom);
		HeaderSize = ImVec2(ImGui::GetItemRectSize().x + 20.0f * m_Zoom, 35 * m_Zoom);
	}

	void Node::DrawInputContainer()
	{
		ImGui::SetCursorPosY(HeaderSize.y + 5 * m_Zoom);
		ImGui::BeginGroup();
		ImGui::Indent(5 * m_Zoom);
		InputContainer.Draw(m_Zoom);
		ImGui::Unindent();
		// ImColor col = m_HeaderColor;
		// ImGui::PushItemWidth(200 * m_Zoom);
		// if (ImGui::ColorEdit4("color", &col.Value.x))
		// {
		// 	m_HeaderColor = col;
		// }
		ImGui::EndGroup();
		ImVec2 containerMin = ImGui::GetItemRectMin();
		ImVec2 containerMax = ImGui::GetItemRectMax() + ImVec2(0, 10 * m_Zoom);
		InputContainerSize = containerMax - containerMin;
		// ImGui::GetWindowDrawList()->AddRectFilled(containerMin, containerMax, IM_COL32(0, 255, 0, 16),0,0);
	}

	void Node::DrawOutputContainer()
	{
		ImGui::SetCursorPosY(HeaderSize.y + 5 * m_Zoom);
		ImGui::SetCursorPosX(InputContainerSize.x + m_InputOutputSpacing);
		ImGui::BeginGroup();
		OutputContainer.Draw(m_Zoom);
		ImGui::EndGroup();
		ImVec2 containerMin = ImGui::GetItemRectMin();
		ImVec2 containerMax = ImGui::GetItemRectMax() + ImVec2(15, 10) * m_Zoom;
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
			m_IsSelected = true;
			drawList->AddRect(ImGui::GetWindowPos(),
							  ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMax(),
							  Style::NodeBorderColorFocused, ImGui::GetStyle().ChildRounding, ImDrawCornerFlags_All,
							  3);
		}
		else if (ImGui::IsWindowHovered())
		{
			m_IsHovered = true;
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

	Ref<VisualComponents::Port> Node::AddPort(VisualComponents::PortDataType dataType, VisualComponents::PortType type, Ref<Float4> data)
	{
		Ref<VisualComponents::Port> port = CreateRef<Port>(this, dataType, type, data);
		if (type == VisualComponents::PortType::Input)
		{
			InputContainer.Add(port);
		}
		else
		{
			OutputContainer.Add(port);
		}
		m_Ports.push_back(port);
		return port;
	}

	void Node::UpdateCoordinates()
	{
		bool canDrag = m_IsDragged || (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(0));
		if (canDrag)
		{
			m_IsDragged = true;
			m_Position = m_Position + ImGui::GetIO().MouseDelta / m_Zoom;
		}
		if (canDrag && ImGui::IsMouseReleased(0))
		{
			m_IsDragged = false;
		}
	}

	void Node::UpdateSize()
	{
		m_InputOutputSpacing = 25 * m_Zoom;
		HeaderSize.x = std::max(glm::max(HeaderSize.x, CustomContentSize.x), InputContainerSize.x + OutputContainerSize.x + m_InputOutputSpacing);
		m_Size.x = HeaderSize.x;
		m_Size.y = HeaderSize.y + CustomContentSize.y + glm::max(InputContainerSize.y, OutputContainerSize.y);
		// HeaderContainer.SetSize(HeaderSize);
		// CustomContentContainer.SetSize(CustomContentSize);
		// InputContainer.SetSize(InputContainerSize);
		// OutputContainer.SetSize(OutputContainerSize);
	}
}
