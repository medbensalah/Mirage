﻿#include "ContentBrowserPanel.h"

#include "Mirage/Definitions/FileExtensions.h"
#include "Mirage/Definitions/DragnDropPayloads.h"
#include "Mirage/Definitions/Icons.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Mirage/Core/Log.h"
#include "Mirage/Core/Time.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"
#include "Mirage/ImGui/Extensions/Splitter.h"

namespace Mirage
{
	// TODO: Change when Project is implemented
	static const std::filesystem::path s_AssetsPath = "Assets";
	static Timer s_BrowserUpdateTimer;
	static Timer s_HierarchyUpdateTimer;
	static float s_BrowserUpdateInterval = 1.0f;

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetsPath)
	{
		MarkUpdateContents();
		m_DirectoryIcon = Texture2D::Create(Icons::Folder);
		m_FileIcon = Texture2D::Create(Icons::File);
		m_MirageIcon = Texture2D::Create(Icons::Mirage);
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if(ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			if (!exists(m_CurrentDirectory))
			{
				m_CurrentDirectory = s_AssetsPath;
				MarkUpdateContents();
			}
			UpdateBrowser();

			DrawHeader();

			m_RightPanelSize = ImGui::GetContentRegionAvail().x - m_LeftPanelSize - m_SeparatorThickness;
			ImGui::DrawSplitter(0, m_SeparatorThickness, &m_LeftPanelSize, &m_RightPanelSize, 50.0f, 100.0f);

			DrawDirectoryHierarchy();

			ImGui::SameLine();

			DrawContents();
			DrawStatusBar();

			ImGui::End();
		}
		else
		{
			ImGui::End();
		}
	}

	void ContentBrowserPanel::DrawHeader()
	{
		bool backward = !m_BackWardNavigation.empty();
		bool forward = !m_ForwardNavigation.empty();

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
		if (!backward)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.3f);
		}
		if (ImGui::Button("<<", ImVec2{0.0f, 25.0f}) || ImGui::IsMouseClicked(3))
		{
			NavigateBackward();
		}
		if (!backward)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}


		ImGui::SameLine();


		if (!forward)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.3f);
		}
		if (ImGui::Button(">>", ImVec2{0.0f, 25.0f}) || ImGui::IsMouseClicked(4))
		{
			NavigateForward();
		}
		if (!forward)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
		ImGui::PopStyleVar();

		ImGui::SameLine();

		static char* pathBuf = new char[512];
		static bool pathBufDirty = false;

		strcpy_s(pathBuf, 512, m_CurrentDirectory.string().c_str());
		ImGui::PushItemWidth(-1);
		pathBufDirty |= ImGui::InputText("##PathHeader", pathBuf, 512);
		ImGui::PopItemWidth();
		if (pathBufDirty && !ImGui::IsItemActive())
		{
			std::filesystem::path newPath = pathBuf;
			if (std::filesystem::exists(newPath))
			{
				pathBufDirty = false;
				NavigateTo(pathBuf);
			}
		}
	}

	void ContentBrowserPanel::DrawDirectoryHierarchy()
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{0.12f, 0.12f, 0.12f, 1.0f});
		ImGui::BeginChild("VerticalView", ImVec2{m_LeftPanelSize, -1}, false, ImGuiWindowFlags_HorizontalScrollbar);
		DrawDirectoryNode(s_AssetsPath, "Assets");
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}

	void ContentBrowserPanel::DrawContents()
	{
		ImGui::BeginGroup();
		auto& style = ImGui::GetStyle();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{m_itemSpacing, style.ItemSpacing.y});
		ImGui::BeginChild("ContentView",
		                  ImVec2{
			                  m_RightPanelSize,
			                  ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeight() - style.WindowPadding.y * 2.0f
		                  },
		                  false);

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
			m_SelectedPath = m_CurrentDirectory;

		float cellSize = m_thumbnailSize + (style.ItemSpacing.x + style.FramePadding.x) * 2.0f;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		int id= 0;
		for (auto& path : m_Entries)
		{
			ImGui::PushID(id++);
			float cellStartX = ImGui::GetCursorPosX();
			float cellWidth = ImGui::GetColumnWidth();
			ImGui::SetCursorPosX(
				cellStartX + (cellWidth - style.FramePadding.x * 2.0f - m_thumbnailSize - style.ItemSpacing.x * 2.0f) /
				2);
			bool isSelected = m_SelectedPath == path;
			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, m_SelectedBGColor);
			}
			
			if (is_directory(path))
			{
				ImGui::ImageButton((ImTextureID)m_DirectoryIcon->GetRendererID(),
				                   {(float)m_thumbnailSize, (float)m_thumbnailSize},
				                   {0, 1}, {1, 0}, -1, ImVec4{0.0f, 0.0f, 0.0f, 0.0f},
				                   m_DirectoryTintColor);
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					NavigateTo(path);
				}
			}
			else
			{
				ImGui::ImageButton((ImTextureID)SetIcon(path)->GetRendererID(),
				                   {(float)m_thumbnailSize, (float)m_thumbnailSize},
				                   {0, 1}, {1, 0}, -1, ImVec4{0.0f, 0.0f, 0.0f, 0.0f},
				                   m_FileTintColor);
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					MRG_CORE_WARN("double clicked file");
					OnFileDoubleClick(path);
				}
			}

			if (isSelected)
			{
				ImGui::PopStyleColor();
			}
			if (ImGui::IsItemClicked())
			{
				m_SelectedPath = path;
			}
			if (ImGui::BeginDragDropSource())
			{
				SetPayload();
				ImGui::BeginTooltip();
				SetTooltip();
				ImGui::EndTooltip();
				ImGui::EndDragDropSource();
			}
			
			std::string fName = path.filename().string();
			float textSize = ImGui::CalcTextSize(fName.c_str()).x;
			float textZoneWidth = cellWidth - style.ItemSpacing.x * 2.0f;
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + cellWidth - style.ItemSpacing.x);
			if (textSize < textZoneWidth)
			{
				ImGui::SetCursorPosX(cellStartX + (textZoneWidth - textSize) / 2);
			}
			else
			{
				ImGui::SetCursorPosX(cellStartX - m_itemSpacing + style.ItemSpacing.x / 2);
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(fName.c_str());
			}

			// Set text max width to cell width - item spacing
			ImGui::Text(fName.c_str());
			ImGui::PopTextWrapPos();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				// TODO: Rename
			}

			ImGui::PopID();
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor();
		ImGui::EndChild();
	}

	void ContentBrowserPanel::DrawStatusBar()
	{
		auto& style = ImGui::GetStyle();
		float settingsWidth = ImGui::CalcTextSize("Thumbnail size: ").x + ImGui::CalcTextSize("Padding: ").x +
			ImGui::GetStyle().ItemSpacing.x * 5.0f + 240.0f;
		float pathWidth = ImGui::GetContentRegionAvail().x - settingsWidth;

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{0.12f, 0.12f, 0.12f, 1.0f});
		ImGui::BeginChild("StatusSelectedPath", ImVec2{pathWidth, -1}, false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SetCursorPos(ImVec2{style.WindowPadding.x, style.WindowPadding.y});
		ImGui::Text(m_SelectedPath.string().c_str());
		ImGui::EndChild();
		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::SetCursorPos(ImVec2{
			ImGui::GetCursorPosX() + style.ItemSpacing.x, ImGui::GetCursorPosY() + style.WindowPadding.y
		});
		ImGui::BeginGroup();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Thumbnail size: ");
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + style.WindowPadding.y);
		ImGui::PushItemWidth(120);
		ImGui::SliderInt("##Thumbnail Size", &m_thumbnailSize, 16, 512);
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + style.WindowPadding.y);
		ImGui::Text("Padding: ");
		ImGui::SameLine();
		ImGui::PushItemWidth(120);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + style.WindowPadding.y);
		ImGui::SliderFloat("##Padding", &m_itemSpacing, 0, 32);
		ImGui::PopItemWidth();
		ImGui::EndGroup();

		ImGui::EndGroup();
	}

	Ref<Texture2D> ContentBrowserPanel::SetIcon(std::filesystem::path path)
	{
		if (path.extension().string() == Extensions::scene)
		{
			return m_MirageIcon;
		}
		return m_FileIcon;
	}

	void ContentBrowserPanel::DrawDirectoryNode(std::filesystem::path path, const char* filename)
	{
		if (is_directory(path))
		{
			ImGuiTreeNodeFlags flags =
				ImGuiTreeNodeFlags_SpanFullWidth |
				ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx(filename, flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				m_SelectedPath = path;
				NavigateTo(path);
			}
			if (opened)
			{
				if (ImGui::IsItemToggledOpen())
				{
					MarkUpdateTree();
				}
				// std::vector<std::filesystem::path> vec = m_BrowseCache[ImGui::GetID(filename)];
				std::vector<std::filesystem::path> vec = Browse(path);
				
				for (auto& p : vec)
				{
					std::string fStr = p.filename().string();
					DrawDirectoryNode(p, fStr.c_str());
				}

				ImGui::TreePop();
			}
		}
	}

	void ContentBrowserPanel::UpdateBrowser()
	{
		if (s_BrowserUpdateTimer.Elapsed() < s_BrowserUpdateInterval && !m_MarkUpdateContents)
		{
			return;
		}

		unsigned int dirs = 0;
		m_Entries.clear();

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();

			if (directoryEntry.is_directory())
			{
				m_Entries.emplace(m_Entries.begin() + dirs++, path);
			}
			else
			{
				m_Entries.emplace_back(path);
			}
		}

		m_MarkUpdateContents = false;
		s_BrowserUpdateTimer.Reset();
	}

	std::vector<std::filesystem::path> ContentBrowserPanel::Browse(const std::filesystem::path& path)
	{
		if (s_HierarchyUpdateTimer.Elapsed() < s_BrowserUpdateInterval && !m_MarkUpdateTree)
		{
			return m_BrowseCache[ImGui::GetID(path.filename().string().c_str())];
		}
		
		std::vector<std::filesystem::path> result;

		for (auto& directoryEntry : std::filesystem::directory_iterator(path))
		{
			const auto& p = directoryEntry.path();

			if (directoryEntry.is_directory())
				result.emplace_back(p);
		}

		m_BrowseCache[ImGui::GetID(path.filename().string().c_str())] = result;
		s_HierarchyUpdateTimer.Reset();

		m_MarkUpdateTree = false;
		return result;
	}

	void ContentBrowserPanel::NavigateTo(const std::filesystem::path& path)
	{
		if (m_CurrentDirectory == path)
			return;

		m_BackWardNavigation.emplace(m_CurrentDirectory);
		m_ForwardNavigation = std::stack<std::filesystem::path>();
		if (is_directory(path))
		{
			m_CurrentDirectory = path;
		}
		else
		{
			std::filesystem::path dir = path.parent_path();
			m_CurrentDirectory = dir;
		}
		m_SelectedPath = path;
		MarkUpdateContents();
	}

	void ContentBrowserPanel::NavigateBackward()
	{
		if (ImGui::IsAnyItemActive())
		{
			ImGui::ClearActiveID();
		}
		
		if (!m_BackWardNavigation.empty())
		{
			m_ForwardNavigation.emplace(m_CurrentDirectory);
			m_CurrentDirectory = m_BackWardNavigation.top();
			m_SelectedPath = m_CurrentDirectory;
			m_BackWardNavigation.pop();
			MarkUpdateContents();
		}
	}

	void ContentBrowserPanel::NavigateForward()
	{
		if (ImGui::IsAnyItemActive())
		{
			ImGui::ClearActiveID();
		}
		
		if (!m_ForwardNavigation.empty())
		{
			m_BackWardNavigation.emplace(m_CurrentDirectory);
			m_CurrentDirectory = m_ForwardNavigation.top();
			m_SelectedPath = m_CurrentDirectory;
			m_ForwardNavigation.pop();
			MarkUpdateContents();
		}
	}

	void ContentBrowserPanel::SetPayload()
	{
		std::string itemPath = m_SelectedPath.string();
		
		const std::string extension = m_SelectedPath.extension().string();

		if ( extension == Extensions::scene)
		{
			ImGui::SetDragDropPayload(Payloads::scene.c_str(), itemPath.c_str(), sizeof(char) * itemPath.size(), ImGuiCond_Once);
		}
		if ( extension == Extensions::texture)
		{
			ImGui::SetDragDropPayload(Payloads::texture.c_str(), itemPath.c_str(), sizeof(char) * itemPath.size());
		}
	}

	void ContentBrowserPanel::SetTooltip()
	{
		bool isDirectory = is_directory(m_SelectedPath);
		const std::string extension = m_SelectedPath.extension().string();
		
		ImGui::Text("Dragging :");
		ImGui::Image(
			(isDirectory
				 ? (ImTextureID)m_DirectoryIcon->GetRendererID()
				 : (ImTextureID)m_FileIcon->GetRendererID()), ImVec2{32, 32}, {0, 1}, {1, 0},
			isDirectory ? m_DirectoryTintColor : m_FileTintColor);
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 16 - ImGui::GetTextLineHeight() / 2);
		ImGui::Text("%s", m_SelectedPath.filename().string().c_str());
	}

	void ContentBrowserPanel::OnFileDoubleClick(std::filesystem::path path)
	{
		const std::string extension = path.extension().string();

		if (extension == Extensions::scene)
		{
			m_RequestedScenePath = path;
			m_IsSceneRequested = true;
		}
	}

}
