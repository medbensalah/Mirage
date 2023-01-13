#include "GraphEditor.h"

#include "glm/ext/vector_common.hpp"
#include "Mirage/Core/Log.h"
#include "Mirage/ImGui/Extensions/Splitter.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"

namespace Mirage::Graph
{
	void GraphEditor::OnImGuiRender(bool* show)
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("GraphEditor", show, flags);

		ApplyStyleChanges();
		Pan();
		Zoom();		
		DrawGrid();
		DrawNodes();
		EndStyleChanges();
		
		ImGui::End();
	}

	void GraphEditor::DrawGrid()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 m_Origin = windowPos;
		ImVec2 canvasSize = windowSize;
		
		//Draw background
		drawList->AddRectFilled(m_Origin, m_Origin + canvasSize, m_BackgroundColor);

		// zoom around mouse
		

		
		float spacing = m_GridSpacing * m_Zoom;
		
		//Draw vertical lines
		for (float x = 0; x <= canvasSize.x + spacing * m_Zoom; x += spacing)
		{
			float offset = fmodf(m_PanOffset.x, spacing);
			ImVec2 p1 = m_Origin + ImVec2(x + offset, 0);
			ImVec2 p2 = m_Origin + ImVec2(x + offset, canvasSize.y);
			drawList->AddLine(p1, p2, m_LineColor, 2.0f);
		}

		//Draw horizontal lines
		for (float y = 0; y <= canvasSize.y + spacing; y += spacing)
		{
			float offset = fmodf(m_PanOffset.y, spacing);
			ImVec2 p1 = m_Origin + ImVec2(0, y + offset);
			ImVec2 p2 = m_Origin + ImVec2(canvasSize.x, y + offset);
			drawList->AddLine(p1, p2, m_LineColor, 2.0f);
		}

		spacing *= 10;

		//Draw thick lines
		for (float x = 0; x <= canvasSize.x + spacing; x += spacing)
		{
			float offset = fmodf(m_PanOffset.x, spacing);
			ImVec2 p1 = m_Origin + ImVec2(x + offset, 0);
			ImVec2 p2 = m_Origin + ImVec2(x + offset, canvasSize.y);
			drawList->AddLine(p1, p2, m_ThickLineColor, 3.0f);
		}

		int i =0;
		for (float y = 0; y <= canvasSize.y +spacing ; y += spacing)
		{
			float offset = fmodf(m_PanOffset.y, spacing);
			ImVec2 p1 = m_Origin + ImVec2(0, y + offset);
			ImVec2 p2 = m_Origin + ImVec2(canvasSize.x, y + offset);
			drawList->AddLine(p1, p2, m_ThickLineColor, 3.0f);
		}
		
	}

	void GraphEditor::DrawNodes()
	{
		ImVec2 canvasPos = ImGui::GetWindowPos();
		ImVec2 offset = canvasPos + m_PanOffset;
		
		for (auto* node : m_NodeList)
		{
			node->Draw(offset, m_Zoom);
		}
	}
	
	void GraphEditor::Pan()
	{
		ImGuiHoveredFlags flags = ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup;
		if (ImGui::IsMouseDragging(2) && ImGui::IsWindowHovered(flags))
		{
			ImVec2 mouseDelta = ImGui::GetMouseDragDelta(2);
			m_PanOffset = m_PanOffset + mouseDelta;
			m_Origin = ImGui::GetCursorScreenPos() + m_PanOffset;
			ImGui::ResetMouseDragDelta(2);
		}
	}

	void GraphEditor::Zoom()
	{
		// mouse wheel
		ImGuiHoveredFlags flags = ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup;
		if (ImGui::IsWindowHovered(flags))
		{
			float zoomDelta = ImGui::GetIO().MouseWheel;
			if (zoomDelta)
			{
				float old = m_Zoom;
				m_Zoom = glm::clamp(old + zoomDelta * m_ZoomFactor, m_MinZoom, m_MaxZoom);
				m_ZoomDelta = m_Zoom - old;
			}
		}
	}

	void GraphEditor::ApplyStyleChanges()
	{
		auto& style = ImGui::GetStyle();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.WindowPadding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.FramePadding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, style.CellPadding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ m_Zoom, m_Zoom });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, style.ItemInnerSpacing * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, style.IndentSpacing * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, style.ScrollbarSize * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, style.GrabMinSize * m_Zoom);
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, style.WindowBorderSize * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, style.ChildBorderSize * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, style.PopupBorderSize * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.FrameBorderSize * m_Zoom);
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, style.WindowRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, style.ChildRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.FrameRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, style.PopupRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, style.ScrollbarRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, style.GrabRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, style.TabRounding * m_Zoom);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.06f, 0.06f, 0.06f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.40f, 0.40f, 0.40f, 1.00f));
	}
	
	void GraphEditor::EndStyleChanges()
	{
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(19);
	}
}
