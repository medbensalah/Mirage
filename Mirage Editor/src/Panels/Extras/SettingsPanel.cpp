#include "SettingsPanel.h"

#include "Mirage/Core/Application.h"
#include "Mirage/Core/Log.h"
#include "Mirage/Definitions/Paths.h"
#include "Mirage/Definitions/Physics.h"
#include "Mirage/Definitions/SettingsSerializer.h"
#include "Mirage/ImGui/Extensions/Splitter.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"

namespace Mirage
{
	void SettingsPanel::OnImGuiRender(bool* show)
	{
		ImGui::SetNextWindowSize(ImVec2{800, 450 }, ImGuiCond_Appearing);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 12.0f, 5.0f });
		ImGui::Begin("SettingsPanel", show);

		m_RightPanelSize = ImGui::GetContentRegionAvail().x - m_LeftPanelSize - m_SeparatorThickness;
		ImGui::DrawSplitter(0, m_SeparatorThickness, &m_LeftPanelSize, &m_RightPanelSize, 50.0f, 100.0f);

		DrawEntries();

		ImGui::SameLine();

		DrawContent();
		
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SettingsPanel::DrawEntries()
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{0.12f, 0.12f, 0.12f, 1.0f});
		ImGui::BeginChild("Entries", ImVec2{m_LeftPanelSize, -1}, false, ImGuiWindowFlags_HorizontalScrollbar);

		for (auto& entry : m_Entries)
		{
			if(ImGui::Selectable(entry.second.c_str(), m_SelectedEntry == entry.first))
			{
				m_SelectedEntry = entry.first;
			}
		}
		
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}

	void SettingsPanel::DrawContent()
	{
		ImGui::BeginGroup();
		ImGui::PushID(m_Entries[m_SelectedEntry].c_str());
		switch (m_SelectedEntry)
		{
		case 0:
			{
				DrawSplitUIItem("Sync with game update", [](){
					return ImGui::Checkbox("##SyncWithGameUpdate", &Physics2D::UpdateEveryFrame);
				});
				
				ImGuiDragDropFlags sliderFlags = ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic;
				if (Physics2D::UpdateEveryFrame)
				{
					sliderFlags = ImGuiSliderFlags_ReadOnly;
					ImGui::GetStyle().Alpha = 0.5f;
				}
				DrawSplitUIItem("Physics update frequency", [&sliderFlags](){
					return ImGui::DragInt("##PhysicsUpdateRate", &Physics2D::FPS, 1, 1,120, "%d", sliderFlags);
				});
				ImGui::GetStyle().Alpha = 1.0f;

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				DrawSplitUIItem("World gravity", [](){
					return ImGui::DragFloat("##PhysicsUpdateRate", &Physics2D::Gravity, 0.05f, 0.0f, 0.0f);
				});
				
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				
				DrawSplitUIItem("Velocity iterations", [](){
					return ImGui::DragInt("##PhysicsUpdateRate", &Physics2D::VelocityIterations, 0.05f, 0.0f, 0.0f);
				});
				DrawSplitUIItem("Position adjustment iterations", [](){
					return ImGui::DragInt("##PhysicsUpdateRate", &Physics2D::PositionIterations, 0.05f, 0.0f, 0.0f);
				});
			}
			break;
		case 1:
			{
				static bool vsync = Application::Get().GetWindow().IsVSync();
				if (DrawSplitUIItem("Vertical synchronization", [](){
					return ImGui::Checkbox("##VSync", &vsync);
				}))
				{
					Application::Get().GetWindow().SetVSync(vsync);
				}
			}
			break;
		}
		ImGui::PopID();
		ImGui::GetStyle().Alpha = 1.0f;
		ImGui::EndGroup();
	}
	
	void SettingsPanel::SaveSettings()
	{
		SettingsSerializer serializer;
		serializer.Serialize(Paths::Settings);
	}

	void SettingsPanel::LoadSettings()
	{
		SettingsSerializer serializer;
		serializer.Deserialize(Paths::Settings);
	}
}
