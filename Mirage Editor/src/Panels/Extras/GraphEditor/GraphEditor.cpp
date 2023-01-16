#include "GraphEditor.h"

#include "glm/ext/vector_common.hpp"
#include "Mirage/Core/Log.h"
#include "Mirage/ImGui/Extensions/Splitter.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"
#include "Mirage/ImGui/Extensions/GraphStyle.h"

namespace Mirage::Graph
{
	void GraphEditor::OnImGuiRender(bool* show)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, Style::BackgroundColor);
		
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		
		if(ImGui::Begin("GraphEditor", show, flags))
		{
			Pan();		
			Zoom();
			DrawGrid();
			DrawDebugInfo();
			ApplyStyleChanges();
			DrawNodes();
			EndStyleChanges();
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);		
	}
	
	void GraphEditor::DrawGrid()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 gridStart = (CanvasStart());
		ImVec2 gridEnd = (CanvasEnd() + Style::GridSpacing * m_Zoom);
		
		float lineThickness = Style::ThinLineWidth * m_Zoom;

		for (float x = gridStart.x; x < gridEnd.x; x += Style::GridSpacing * m_Zoom)
		{
			float offset = fmodf(m_Pan.x, Style::GridSpacing * m_Zoom);
			drawList->AddLine(ImVec2(x + offset, gridStart.y), ImVec2(x + offset, gridEnd.y), Style::ThinLineColor, lineThickness);
		}
		for (float y = gridStart.y; y < gridEnd.y; y += Style::GridSpacing * m_Zoom)
		{
			float offset = fmodf(m_Pan.y, Style::GridSpacing * m_Zoom);
			drawList->AddLine(ImVec2(gridStart.x, y + offset), ImVec2(gridEnd.x, y + offset), Style::ThinLineColor, lineThickness);
		}
	}

	void GraphEditor::DrawNodes()
	{
		ImVec2 start = CanvasStart() + m_Pan;
		for (auto* node : m_NodeList)
		{
			node->OnImGuiRender(start, m_Zoom);
		}
	}
	
	void GraphEditor::Pan()
	{
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && ImGui::IsMouseDragging(2))
		{
			m_Pan = m_Pan + ImGui::GetIO().MouseDelta;
		}
	}

	void GraphEditor::Zoom()
	{
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && ImGui::GetIO().MouseWheel != 0.0f)
		{
			auto start = ScreenToCanvasPosition(ImGui::GetMousePos());

			float oldZoom = m_Zoom;
			m_Zoom += ImGui::GetIO().MouseWheel * m_ZoomFactor;
			m_Zoom = std::clamp(m_Zoom, m_MinZoom, m_MaxZoom);
			float zoomDelta = m_Zoom - oldZoom;
		
			auto end = ScreenToCanvasPosition(ImGui::GetMousePos());
			m_Pan = m_Pan - (start - end) * m_Zoom;
		}
	}
	
	ImVec2 GraphEditor::CanvasStart()
	{
		return ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos();
	}

	ImVec2 GraphEditor::CanvasEnd()
	{
		ImVec2 canvasStart = CanvasStart();
		return ImGui::GetContentRegionAvail() + canvasStart;
	}
	
	void GraphEditor::ApplyStyleChanges()
	{
		auto& style = ImGui::GetStyle();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ m_Zoom, m_Zoom });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.FramePadding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, style.CellPadding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ m_Zoom, m_Zoom });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, style.ItemInnerSpacing * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, style.IndentSpacing * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, style.ScrollbarSize * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, style.GrabMinSize * m_Zoom);
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, style.WindowBorderSize * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, style.PopupBorderSize * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, style.FrameBorderSize * m_Zoom);
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, style.WindowRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, style.ChildRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.FrameRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, style.PopupRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, style.ScrollbarRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, style.GrabRounding * m_Zoom);
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, style.TabRounding * m_Zoom);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, Style::NodeColor);
	}
	
	void GraphEditor::EndStyleChanges()
	{
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(19);
	}

	ImVec2 GraphEditor::ScreenToCanvasPosition(const ImVec2& screenPos)
	{
		return (screenPos - CanvasStart() - m_Pan) / m_Zoom;
	}

	ImVec2 GraphEditor::CanvasToScreenPosition(const ImVec2& graphPos)
	{
		return (graphPos * m_Zoom) + CanvasStart() + m_Pan;
	}


#pragma region DEBUG

	void GraphEditor::DrawDebugInfo()
	{
		// ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() +  ImVec2(0, 25));
	}
	
	void GraphEditor::DrawDebugPoint(ImVec2 center,float thickness, ImU32 col)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 start = CanvasToScreenPosition(center);
		drawList->AddCircleFilled(start, thickness * m_Zoom, col);
	}

	void GraphEditor::DrawDebugCircle(ImVec2 pos, float thickness, float radius, ImU32 col)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 start = CanvasToScreenPosition(pos);
		drawList->AddCircle(start, radius * m_Zoom, col, 0, thickness * m_Zoom);
	}

	void GraphEditor::DrawDebugText(const char* fmt, ...)
	{
		ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() +  ImVec2(10, 0));
		va_list args;
		va_start(args, fmt);
		ImGui::TextColoredV(ImVec4( 0.41f, 0.85f, 0.97f, 1), fmt, args);
		va_end(args);
	}
#pragma endregion 
}
