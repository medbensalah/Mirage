#pragma once
#include <string>

#include "ImGui/imgui.h"
#include "Mirage/Core/Core.h"
#include "VisualComponents/VisualComponentsInclude.h"

namespace Mirage
{
	namespace VisualComponents
	{
		enum class PortDataType;
		enum class PortType;
	}

	class Texture2D;
}

namespace Mirage::Graph
{
	class Node
	{
		friend class VisualComponents::Port;
	public:
		friend class GraphEditor;
		
		ImVec2 HeaderSize;
		ImVec2 InputContainerSize;
		ImVec2 OutputContainerSize;
		ImVec2 CustomContentSize;

		VisualComponents::Container HeaderContainer;
		VisualComponents::Container InputContainer;
		VisualComponents::Container OutputContainer;
		VisualComponents::Container CustomContentContainer;
		
		Node(GraphEditor* graph, std::string title, ImVec2 position);
		~Node() = default;

		void OnImGuiRender(ImVec2 origin, float zoom);

		//getters and setters
		std::string GetTitle() const { return m_Title; }
		void SetTitle(std::string title) { m_Title = title; }

		ImVec2 GetPosition() const { return m_Position; }
		void SetPosition(ImVec2 position) { m_Position = position; }

		ImVec2 GetSize() const { return m_Size; }
		void SetSize(ImVec2 size) { m_Size = size; }

	private:
		void UpdateCoordinates();
		void UpdateSize();

		void DrawHeader();
		void DrawInputContainer();
		void DrawOutputContainer();
		void DrawCustomContent();
		void DrawBorder();

		Ref<VisualComponents::Port> AddPort(VisualComponents::PortDataType dataType, VisualComponents::PortType type);
	
	private:
		static Ref<Texture2D> bg;
		
		std::string m_Title;
		ImVec2 m_Position;
		ImVec2 m_Size;
		float m_Zoom;
		float m_InputOutputSpacing;

		ImU32 m_HeaderColor = IM_COL32(0, 0, 255, 196);

		bool m_IsSelected = false;
		bool m_IsHovered = false;
		bool m_IsDragged = false;

		std::vector<Ref<VisualComponents::Port>>  m_Ports {};
		// std::vector<Ref<VisualComponents::Link>> m_Links {};

		GraphEditor* m_Graph;
		
		static bool Initialized;
	};
}
