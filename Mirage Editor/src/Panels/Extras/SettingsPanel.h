#pragma once

#include <string>
#include <unordered_map>

namespace Mirage
{
	class SettingsPanel
	{
	public:
		SettingsPanel() = default;

		void OnImGuiRender(bool* show);

		void SaveSettings();
		void LoadSettings();
		
	private:
		void DrawEntries();
		void DrawContent();
		
	private:
		float m_LeftPanelSize = 175.0f;
		float m_RightPanelSize;
		float m_SeparatorThickness = 8.0f;

		std::unordered_map<int, std::string> m_Entries =
		{
			{0, "Physics"},
			{1, "Graphics"},
			{2, "Audio"},
			{3, "Input"},
			{4, "Network"},
			{5, "Gameplay"},
			{6, "Editor"},
			{7, "Debug"}
		};
		int m_SelectedEntry = 0;
	};
}
