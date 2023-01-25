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
			// TODO: change into Ref<>
			m_NodeList.push_back(new Node(this, "Node 1", ImVec2(100, 100)));
			m_NodeList.push_back(new Node(this, "Node 2", ImVec2(400, 250)));
		}

		void OnImGuiRender(bool* show);

		ImVec2 CanvasStart();
		ImVec2 CanvasEnd();

		inline void AddLink(Ref<VisualComponents::Link> linkRef)
		{
			m_LinksList.push_back(linkRef);
		}
		inline void RemoveLink(Ref<VisualComponents::Link> linkRef)
		{
			m_LinksList.erase(std::remove(m_LinksList.begin(), m_LinksList.end(), linkRef), m_LinksList.end());
		}

	private:
		void DrawGrid();
		void DrawNodes();
		void DrawLinks();
		
		void Pan();
		void Zoom();
		
		void ApplyStyleChanges();
		void EndStyleChanges();

		ImVec2 ScreenToCanvasPosition(const ImVec2& screenPos);
		ImVec2 CanvasToScreenPosition(const ImVec2& graphPos);
		
		// ------------------- Debug -------------------
		void DrawDebugInfo();
		void DrawDebugPoint(ImVec2 pos, float thickness = 5.0f, ImU32 col = IM_COL32(0, 255, 0, 255));
		void DrawDebugCircle(ImVec2 pos, float thickness = 1.0f, float radius = 5.0f, ImU32 col = IM_COL32(0, 255, 0, 255));
		void DrawDebugText(const char* fmt, ...);
	
	private:
		std::vector<Node*> m_NodeList {}; 
		std::vector<Ref<VisualComponents::Link>> m_LinksList {}; 
		
		
		float m_Zoom = 1.0f;
		float m_ZoomFactor = 0.125f;
		float m_MinZoom = 0.2f;
		float m_MaxZoom = 3.0f;
		ImVec2 m_ZoomOrigin = ImVec2(0.0f, 0.0f);
		
		ImVec2 m_Pan = ImVec2(0.0f, 0.0f);
	};
}
