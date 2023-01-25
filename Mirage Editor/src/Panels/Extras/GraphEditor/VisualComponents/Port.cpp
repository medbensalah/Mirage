#include "Port.h"

#include "ImageButton.h"
#include "../GraphEditor.h"
#include "ImGui/imgui_internal.h"
#include "Mirage/Definitions/Textures.h"

namespace Mirage::VisualComponents
{
	bool Port::Initialized = false;
	Ref<Texture2D> Port::s_PortTexture;
	Port* Port::s_HoveredPort = nullptr;

	std::unordered_map<PortDataType, ImColor> Port::s_Colors {
		{ PortDataType::Bool, 			ImColor(196, 0, 0) },
		
		{ PortDataType::Int,				ImColor(50, 200, 225) },
		{ PortDataType::Int2, 			ImColor(50, 130, 225) },
		{ PortDataType::Int3, 			ImColor(50, 70, 225) },
		{ PortDataType::Int4, 			ImColor(50, 25, 160) },
		
		{ PortDataType::Float,			ImColor(50, 225, 135) },
		{ PortDataType::Float2,			ImColor(50, 190, 90) },
		{ PortDataType::Float3,			ImColor(50, 150, 50) },
		{ PortDataType::Float4,			ImColor(50, 100, 50) },

		
		{ PortDataType::Color,			ImColor(200, 100, 200) },
		
		{ PortDataType::String,			ImColor(225, 190, 90) },
		// { PortDataType::TextureCube, ImColor(255, 255, 255) },
		// { PortDataType::Texture3D, ImColor(255, 255, 255) },
		// { PortDataType::Texture2DArray, ImColor(255, 255, 255) },
		// { PortDataType::TextureCubeArray, ImColor(255, 255, 255) },
		// { PortDataType::Sampler2D, ImColor(255, 255, 255) },
		// { PortDataType::SamplerCube, ImColor(255, 255, 255) },
		// { PortDataType::Sampler3D, ImColor(255, 255, 255) },
		// { PortDataType::Sampler2DArray, ImColor(255, 255, 255) },
		// { PortDataType::SamplerCubeArray, ImColor(255, 255, 255) },
		// { PortDataType::Matrix4x4, ImColor(255, 255, 255) },
		// { PortDataType::Matrix3x3, ImColor(255, 255, 255) },
		// { PortDataType::Matrix2x2, ImColor(255, 255, 255) },
		// { PortDataType::Matrix4x3, ImColor(255, 255, 255) },
	};
	
	Port::Port(Graph::Node* owner, PortDataType dataType, PortType portType)
		: m_Owner(owner), m_DataType(dataType), m_PortType(portType)
	{
		if (!Initialized)
		{
			s_PortTexture = Texture2D::Create(Textures::NodePort);
			Initialized = true;
		}
		m_Color = s_Colors[dataType];

		if (m_PortType == PortType::Input)
			Container::Add(new SameLineSpacing(15.0f));
		
		Add(new Color(m_Color));
		if (m_PortType == PortType::Output)
			Container::Add(new SameLineSpacing(15.0f));
		
	}

	void Port::Draw(float scale)
	{
		if (m_PortType == PortType::Input)
		{
			DrawPort(scale);
		}
		
		for (auto& component : m_Components)
		{
			component->Draw(scale);
		}

		if (m_PortType == PortType::Output)
		{
			DrawPort(scale);
		}
		
		if (m_Dragging)
		{
			OnDrag(scale);
		}
	}

	void Port::DrawPort(float scale)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1,1,1,0.2f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0.7f});
		ImGui::ImageButton((ImTextureID)s_PortTexture->GetRendererID(), {24 * scale, 24 * scale},
			ImVec2{0, 0}, ImVec2{1, 1}, 0,
			ImColor(255,255,255,0), m_Color);
		ImGui::PopStyleColor(3);
		m_PortPos = ImGui::GetItemRectMin();
		m_PortPos.x += 12 * scale;
		m_PortPos.y += 12 * scale;
		if (s_HoveredPort != this == ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		{
			s_HoveredPort = this;
		}
		if (s_HoveredPort == this && !ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		{
			s_HoveredPort = nullptr;
		}
		
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
		{
			OnBeginDrag(scale);
		}
		if (m_Dragging && ImGui::IsMouseReleased(0))
		{
			OnEndDrag(scale);
		}
	}

	void Port::OnBeginDrag(float scale)
	{
		m_Dragging = true;
		
		Ref<Link> link = CreateRef<Link>();
		link->SetPort(this);
		link->SetColor(ImColor{1.0f, 1.0f, 1.0f, 0.5f});
		m_Links.push_back(link);
		m_Owner->m_Graph->AddLink(link);
	}

	void Port::OnDrag(float scale)
	{
		
	}

	void Port::OnEndDrag(float scale)
	{
		if (s_HoveredPort)
		{
			// TODO: condition is a lambda passed by the user
			if (s_HoveredPort->m_PortType != m_PortType && s_HoveredPort->m_DataType == m_DataType)
			{
				m_Links.back()->SetPort(s_HoveredPort);
				m_Links.back()->SetColor(m_Color);
			}
			else
			{
				m_Owner->m_Graph->RemoveLink(m_Links.back());
				m_Links.pop_back();
			}
		}
		else
		{
			m_Owner->m_Graph->RemoveLink(m_Links.back());
			m_Links.pop_back();
		}
		m_Dragging = false;
	}
}
