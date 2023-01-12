#pragma once
#include <vector>

#include "Node.h"
#include "ImGui/imgui.h"
#include "Mirage/Math/glmTypes.h"

namespace Mirage::Graph
{
	class GraphEditor
	{
	public:
		GraphEditor()
		{
			m_NodeList.push_back(new Node("new node", ImVec2(100, 100)));
		}

		void OnImGuiRender(bool* show);

	private:
		void DrawGrid();
		void DrawNodes();
		void Pan();
		void Zoom();
		
	private:
		std::vector<Node*> m_NodeList {}; 
		
		/***************** INTERNAL DATA *****************/
		float m_GridSpacing = 25.0f;
		ImColor m_BackgroundColor = ImColor(27, 27, 27, 255);
		ImColor m_LineColor = ImColor(40, 40, 40, 255);
		ImColor m_ThickLineColor = ImColor(22, 22, 22, 255);

		float m_Zoom = 1.0f;
		ImVec2 m_PanOffset = ImVec2(0.0f, 0.0f);
	};
}
