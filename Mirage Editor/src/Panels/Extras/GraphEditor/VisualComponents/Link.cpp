#include "Link.h"
#include "Port.h"

namespace Mirage::VisualComponents
{
	static ImVec2 SampleBezier(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, float t)
	{
		float t2 = t * t;
		float t3 = t2 * t;
		float u = 1.0f - t;
		float u2 = u * u;
		float u3 = u2 * u;

		return {
			u3 * p1.x + 3 * u2 * t * p2.x + 3 * u * t2 * p3.x + t3 * p4.x,
			u3 * p1.y + 3 * u2 * t * p2.y + 3 * u * t2 * p3.y + t3 * p4.y
		};
	}

	static bool ImLineSegmentTest(const ImVec2& a, const ImVec2& b, const ImVec2& p, float thickness)
	{
		ImVec2 d = {b.x - a.x, b.y - a.y};
		ImVec2 f = {a.x - p.x, a.y - p.y};

		float a1 = d.x * d.x + d.y * d.y;
		float b1 = 2.0f * (f.x * d.x + f.y * d.y);
		float c1 = (f.x * f.x + f.y * f.y) - thickness * thickness;

		float det = b1 * b1 - 4.0f * a1 * c1;
		if (det < 0.0f)
			return false;
		det = sqrtf(det);

		float t1 = (-b1 - det) / (2.0f * a1);
		if (t1 >= 0.0f && t1 <= 1.0f)
			return true;

		float t2 = (-b1 + det) / (2.0f * a1);
		if (t2 >= 0.0f && t2 <= 1.0f)
			return true;

		return false;
	}

	bool Link::IsHovered(float scale)
	{
		if (ImGui::IsAnyItemHovered()) return false;

		ImVec2 mouse = ImGui::GetMousePos();
		//sample bezier curve
		ImVec2 lastPoint = m_Start;
		int samples = 5;
		for (int i = 1; i <= samples; i++)
		{
			float t = i * 1.0f / samples;
			ImVec2 point = SampleBezier(m_Start, m_StartTangent, m_EndTangent, m_End, t);
			if (ImLineSegmentTest(lastPoint, point, mouse, 5.0f * scale))
			{
				return true;
				break;
			}
			lastPoint = point;
		}

		return false;
	}

	void Link::SetPort(Port* port)
	{
		switch (port->GetPortType())
		{
		case PortType::Output:
			m_InputPort = port;
			m_Start = port->GetPosition();
			break;
		case PortType::Input:
			m_OutputPort = port;
			m_End = port->GetPosition();
			break;
		}

		ForwardData();
	}

	void Link::SetColor(const ImColor& color)
	{
		m_Color = color;
	}

	void Link::ForwardData()
	{
		if (m_InputPort && m_OutputPort)
		{
			// m_InputPort->(m_OutputPort->GetData());
		}
	}

	void Link::Draw(float scale)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		bool hovered = IsHovered(scale);
		ImColor col = hovered ? m_HoveredColor : m_Color;
		float thickness = hovered ? m_HoveredThickness : m_Thickness;

		m_Start = m_InputPort ? m_InputPort->GetPosition() : ImGui::GetMousePos();
		m_End = m_OutputPort ? m_OutputPort->GetPosition() : ImGui::GetMousePos();
		
		if (!(m_OutputPort && m_InputPort))
		{
			col = m_GrayedColor;
			thickness = m_GrayedThickness;
		}
		
		m_StartTangent = {m_Start.x + m_TgLength * scale, m_Start.y};
		m_EndTangent = {m_End.x - m_TgLength * scale, m_End.y};
		
		drawList->AddBezierCubic(m_Start, m_StartTangent, m_EndTangent, m_End, col, thickness * scale);
	}
}
