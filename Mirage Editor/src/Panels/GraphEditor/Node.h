#pragma once
#include <string>
#include <vector>

#include "ImGui/imgui.h"
#include "VisualComponents/Text.h"
#include "VisualComponents/VisualComponent.h"

namespace Mirage::Graph
{
	class Node
	{
	public:
		Node(std::string title, ImVec2 position);
		~Node() = default;

		void Draw(ImVec2 offset, float zoom);

		//getters and setters
		std::string GetTitle() const { return m_Title; }
		void SetTitle(std::string title) { m_Title = title; }

		ImVec2 GetPosition() const { return m_Position; }
		void SetPosition(ImVec2 position) { m_Position = position; }

		ImVec2 GetSize() const { return m_Size; }
		void SetSize(ImVec2 size) { m_Size = size; }

	private:
		std::vector<VisualComponents::VisualComponent*> m_VisualComponents;
		
		std::string m_Title;
		ImVec2 m_Position;
		ImVec2 m_Size;
		float m_Scale;
	};
}
