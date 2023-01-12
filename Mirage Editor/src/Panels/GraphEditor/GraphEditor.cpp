#include "GraphEditor.h"

#include "glm/ext/vector_common.hpp"
#include "Mirage/Core/Log.h"
#include "Mirage/ImGui/Extensions/Splitter.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"

namespace Mirage::Graph
{
	void GraphEditor::OnImGuiRender(bool* show)
	{
		ImGui::Begin("GraphEditor", show);

		Pan();
		Zoom();
		DrawGrid();
		DrawNodes();
		
		ImGui::End();
	}

	void GraphEditor::DrawGrid()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 canvasPos = windowPos;
		ImVec2 canvasSize = windowSize;
		
		//Draw background
		drawList->AddRectFilled(canvasPos, canvasPos + canvasSize, m_BackgroundColor);

		float spacing = m_GridSpacing * m_Zoom;
		//Draw vertical lines
		for (float x = 0; x <= canvasSize.x + spacing * m_Zoom; x += spacing)
		{
			float offset = fmodf(m_PanOffset.x, spacing);
			ImVec2 p1 = canvasPos + ImVec2(x + offset, 0);
			ImVec2 p2 = canvasPos + ImVec2(x + offset, canvasSize.y);
			drawList->AddLine(p1, p2, m_LineColor, 2.0f);
		}

		//Draw horizontal lines
		for (float y = 0; y <= canvasSize.y + spacing; y += spacing)
		{
			float offset = fmodf(m_PanOffset.y, spacing);
			ImVec2 p1 = canvasPos + ImVec2(0, y + offset);
			ImVec2 p2 = canvasPos + ImVec2(canvasSize.x, y + offset);
			drawList->AddLine(p1, p2, m_LineColor, 2.0f);
		}

		spacing *= 10;

		//Draw thick lines
		for (float x = 0; x <= canvasSize.x + spacing; x += spacing)
		{
			float offset = fmodf(m_PanOffset.x, spacing);
			ImVec2 p1 = canvasPos + ImVec2(x + offset, 0);
			ImVec2 p2 = canvasPos + ImVec2(x + offset, canvasSize.y);
			drawList->AddLine(p1, p2, m_ThickLineColor, 3.0f);
		}

		int i =0;
		for (float y = 0; y <= canvasSize.y +spacing ; y += spacing)
		{
			float offset = fmodf(m_PanOffset.y, spacing);
			ImVec2 p1 = canvasPos + ImVec2(0, y + offset);
			ImVec2 p2 = canvasPos + ImVec2(canvasSize.x, y + offset);
			drawList->AddLine(p1, p2, m_ThickLineColor, 3.0f);
		}
		
	}

	void GraphEditor::DrawNodes()
	{
		ImGui::SetWindowFontScale(m_Zoom);
		ImVec2 canvasPos = ImGui::GetWindowPos();
		ImVec2 offset = canvasPos + m_PanOffset;
		for (auto* node : m_NodeList)
		{
			node->Draw(offset, m_Zoom);
		}
		ImGui::SetWindowFontScale(1);
		for (auto* node : m_NodeList)
		{
			node->Draw(offset + ImVec2{150.0f, 150.0f}, m_Zoom);
		}
	}

	void GraphEditor::Pan()
	{
		if (ImGui::IsMouseDragging(2) && ImGui::IsWindowHovered())
		{
			ImVec2 mouseDelta = ImGui::GetMouseDragDelta(2);
			m_PanOffset.x += mouseDelta.x;
			m_PanOffset.y += mouseDelta.y;
			ImGui::ResetMouseDragDelta(2);
		}
	}

	void GraphEditor::Zoom()
	{
		// mouse wheel
		if (ImGui::IsWindowHovered())
		{
			m_Zoom += ImGui::GetIO().MouseWheel * 0.01f;
			m_Zoom = glm::clamp(m_Zoom, 0.1f, 10.0f);
		}
	}
}
