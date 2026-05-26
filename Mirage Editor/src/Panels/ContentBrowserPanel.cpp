#include "ContentBrowserPanel.h"

#include "Mirage/Definitions/FileExtensions.h"
#include "Mirage/Definitions/DragnDropPayloads.h"
#include "Mirage/Definitions/Icons.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Mirage/Core/Log.h"
#include "Mirage/Core/Time.h"
#include "Mirage/ImGui/Extensions/DrawingAPI.h"
#include "Mirage/ImGui/Extensions/Splitter.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include <stb_Image/stb_image.h>

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
			HandleShortcuts();

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

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
		{
			m_SelectedPath = m_CurrentDirectory;
			m_SelectedPaths.clear();
			if (!m_RenameTarget.empty())
				m_RenameTarget.clear();
		}

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
			bool isMultiSelected = IsSelected(path);
			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, m_SelectedBGColor);
			}
			else if (isMultiSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{m_SelectedBGColor.x, m_SelectedBGColor.y, m_SelectedBGColor.z, 0.65f});
			}
			bool isCutVisual = (m_ClipboardMode == ClipboardMode::Cut) &&
				(std::find(m_ClipboardPaths.begin(), m_ClipboardPaths.end(), path) != m_ClipboardPaths.end());
			if (isCutVisual)
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.45f);
			
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
				Ref<Texture2D> displayTexture = SetIcon(path);
				if (IsThumbnailCandidate(path))
				{
					int bucket = GetThumbnailBucket();
					Ref<Texture2D> thumb = GetThumbnailTexture(path, bucket);
					if (thumb && thumb->IsLoaded())
						displayTexture = thumb;
				}
				ImGui::ImageButton((ImTextureID)displayTexture->GetRendererID(),
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
			else if (isMultiSelected)
			{
				ImGui::PopStyleColor();
			}
			if (isCutVisual)
				ImGui::PopStyleVar();
			if (ImGui::IsItemClicked())
			{
				m_SelectedPath = path;
				bool ctrl = ImGui::GetIO().KeyCtrl;
				bool shift = ImGui::GetIO().KeyShift;
				if (shift)
				{
					SelectRangeTo(path);
				}
				else if (ctrl)
				{
					if (IsSelected(path))
						m_SelectedPaths.erase(path);
					else
						m_SelectedPaths.insert(path);
				}
				else
				{
					SelectSingle(path);
				}
				if (!m_RenameTarget.empty() && !IsSelected(m_RenameTarget))
					m_RenameTarget.clear();
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				m_SelectedPath = path;
				if (!IsSelected(path))
					SelectSingle(path);
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
			if (m_RenameTarget == path)
			{
				if (m_RenameFocusPending)
				{
					ImGui::SetKeyboardFocusHere();
					m_RenameFocusPending = false;
				}
				bool commit = ImGui::InputText("##RenameInline", m_RenameBuffer, sizeof(m_RenameBuffer),
					ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
				bool cancel = ImGui::IsItemActive() && ImGui::IsKeyPressed(ImGuiKey_Escape);
				if (commit || (!ImGui::IsItemActive() && ImGui::IsItemDeactivatedAfterEdit()))
				{
					std::string newName = m_RenameBuffer;
					if (!newName.empty())
					{
						auto dst = m_RenameTarget.parent_path() / newName;
						if (dst != m_RenameTarget)
						{
							std::error_code ec;
							std::filesystem::rename(m_RenameTarget, dst, ec);
						if (!ec)
						{
							InvalidateThumbnail(m_RenameTarget);
							m_SelectedPath = dst;
							SelectSingle(dst);
						}
						}
					}
					m_RenameTarget.clear();
					MarkUpdateContents();
					MarkUpdateTree();
				}
				else if (cancel)
				{
					m_RenameTarget.clear();
				}
			}
			else
			{
				ImGui::Text(fName.c_str());
			}
			ImGui::PopTextWrapPos();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				// TODO: Rename
			}

			ImGui::PopID();
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		DrawContextMenu();

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

	void ContentBrowserPanel::DrawContextMenu()
	{
		if (ImGui::BeginPopupContextWindow("ContentBrowserContext", ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem("Create Folder"))
			{
				auto dir = m_CurrentDirectory / ("New Folder " + std::to_string(m_NewFolderCounter++));
				while (exists(dir))
					dir = m_CurrentDirectory / ("New Folder " + std::to_string(m_NewFolderCounter++));
				std::filesystem::create_directory(dir);
				m_SelectedPath = dir;
				SelectSingle(dir);
				BeginRename(dir);
				MarkUpdateContents();
				MarkUpdateTree();
			}

			bool hasSelection = !m_SelectedPaths.empty();
			if (!hasSelection)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.45f);
			}
			if (ImGui::MenuItem("Copy"))
			{
				m_ClipboardPaths.assign(m_SelectedPaths.begin(), m_SelectedPaths.end());
				m_ClipboardMode = ClipboardMode::Copy;
			}
			if (ImGui::MenuItem("Cut"))
			{
				m_ClipboardPaths.assign(m_SelectedPaths.begin(), m_SelectedPaths.end());
				m_ClipboardMode = ClipboardMode::Cut;
			}
			if (ImGui::MenuItem("Rename") && !m_SelectedPaths.empty())
			{
				BeginRename(*m_SelectedPaths.begin());
			}
			if (ImGui::MenuItem("Delete"))
			{
				QueueDeleteSelection();
			}
			if (ImGui::MenuItem("Open in File Explorer") && !m_SelectedPaths.empty())
			{
				OpenInFileExplorer(*m_SelectedPaths.begin());
			}
			if (!hasSelection)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			bool canPaste = !m_ClipboardPaths.empty();
			if (!canPaste)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.45f);
			}
			if (ImGui::MenuItem("Paste"))
			{
				PasteTo(m_CurrentDirectory);
			}
			if (!canPaste)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
			ImGui::EndPopup();
		}

		if (!m_DeletePending.empty())
			ImGui::OpenPopup("ConfirmDeletePopup");
		if (ImGui::BeginPopupModal("ConfirmDeletePopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Delete %d selected item(s)?", (int)m_DeletePending.size());
			ImGui::Spacing();
			if (ImGui::Button("Delete", ImVec2(120, 0)))
			{
				for (const auto& p : m_DeletePending)
					RemovePath(p);
				InvalidateThumbnail(m_DeletePending);
				m_DeletePending.clear();
				m_SelectedPaths.clear();
				m_SelectedPath = m_CurrentDirectory;
				MarkUpdateContents();
				MarkUpdateTree();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				m_DeletePending.clear();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void ContentBrowserPanel::OnExternalFilesDropped(const std::vector<std::string>& paths)
	{
		for (const auto& p : paths)
		{
			std::filesystem::path src = p;
			if (!exists(src))
				continue;
			auto dst = MakeUniquePath(m_CurrentDirectory / src.filename());
			try
			{
				if (is_directory(src))
					std::filesystem::copy(src, dst, std::filesystem::copy_options::recursive);
				else
					std::filesystem::copy_file(src, dst, std::filesystem::copy_options::overwrite_existing);
			}
			catch (...)
			{
				MRG_CORE_WARN("Failed to import dropped path: {0}", src.string());
			}
		}
		for (const auto& p : paths)
			InvalidateThumbnail(m_CurrentDirectory / std::filesystem::path(p).filename());
		MarkUpdateContents();
		MarkUpdateTree();
	}

	void ContentBrowserPanel::QueueDeleteSelection()
	{
		m_DeletePending.assign(m_SelectedPaths.begin(), m_SelectedPaths.end());
	}

	void ContentBrowserPanel::PasteTo(const std::filesystem::path& targetDirectory)
	{
		for (const auto& src : m_ClipboardPaths)
		{
			if (!exists(src))
				continue;
			auto dst = MakeUniquePath(targetDirectory / src.filename());
			try
			{
				if (is_directory(src))
					std::filesystem::copy(src, dst, std::filesystem::copy_options::recursive);
				else
					std::filesystem::copy_file(src, dst, std::filesystem::copy_options::overwrite_existing);
				if (m_ClipboardMode == ClipboardMode::Cut)
				{
					RemovePath(src);
					InvalidateThumbnail(src);
				}
			}
			catch (...)
			{
				MRG_CORE_WARN("Failed paste path: {0}", src.string());
			}
		}
		if (m_ClipboardMode == ClipboardMode::Cut)
		{
			m_ClipboardPaths.clear();
			m_ClipboardMode = ClipboardMode::None;
		}
		MarkUpdateContents();
		MarkUpdateTree();
	}

	std::filesystem::path ContentBrowserPanel::MakeUniquePath(const std::filesystem::path& candidate) const
	{
		if (!exists(candidate))
			return candidate;
		std::filesystem::path dir = candidate.parent_path();
		std::string stem = candidate.stem().string();
		std::string ext = candidate.extension().string();
		int index = 1;
		std::filesystem::path out;
		do
		{
			out = dir / (stem + " (" + std::to_string(index++) + ")" + ext);
		} while (exists(out));
		return out;
	}

	void ContentBrowserPanel::RemovePath(const std::filesystem::path& path)
	{
		std::error_code ec;
		std::filesystem::remove_all(path, ec);
		if (ec)
			MRG_CORE_WARN("Failed to delete: {0}", path.string());
	}

void ContentBrowserPanel::SelectSingle(const std::filesystem::path& path)
	{
		m_SelectedPaths.clear();
		m_SelectedPaths.insert(path);
		m_SelectionAnchor = path;
	}

	bool ContentBrowserPanel::IsSelected(const std::filesystem::path& path) const
	{
		return m_SelectedPaths.find(path) != m_SelectedPaths.end();
	}

	void ContentBrowserPanel::SelectRangeTo(const std::filesystem::path& path)
	{
		if (m_SelectionAnchor.empty())
		{
			SelectSingle(path);
			return;
		}

		int a = -1, b = -1;
		for (int i = 0; i < (int)m_Entries.size(); i++)
		{
			if (m_Entries[i] == m_SelectionAnchor) a = i;
			if (m_Entries[i] == path) b = i;
		}
		if (a < 0 || b < 0)
		{
			SelectSingle(path);
			return;
		}
		if (a > b) std::swap(a, b);
		m_SelectedPaths.clear();
		for (int i = a; i <= b; i++)
			m_SelectedPaths.insert(m_Entries[i]);
		m_SelectedPath = path;
	}

	void ContentBrowserPanel::BeginRename(const std::filesystem::path& path)
	{
		m_RenameTarget = path;
		m_RenameFocusPending = true;
		memset(m_RenameBuffer, 0, sizeof(m_RenameBuffer));
		const std::string name = path.filename().string();
		strncpy_s(m_RenameBuffer, sizeof(m_RenameBuffer), name.c_str(), _TRUNCATE);
	}

	void ContentBrowserPanel::OpenInFileExplorer(const std::filesystem::path& path)
	{
#ifdef _WIN32
		std::string command = "explorer /select,\"" + path.string() + "\"";
		system(command.c_str());
#endif
	}

	void ContentBrowserPanel::HandleShortcuts()
	{
		if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
			return;

		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsKeyPressed(ImGuiKey_F2) && !m_SelectedPaths.empty())
		{
			BeginRename(*m_SelectedPaths.begin());
			return;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_Delete) && !m_SelectedPaths.empty() && m_RenameTarget.empty())
		{
			QueueDeleteSelection();
			return;
		}
		if (!io.KeyCtrl)
			return;

		if (ImGui::IsKeyPressed(ImGuiKey_A))
		{
			m_SelectedPaths.clear();
			for (auto& p : m_Entries)
				m_SelectedPaths.insert(p);
			if (!m_Entries.empty())
			{
				m_SelectedPath = m_Entries.back();
				m_SelectionAnchor = m_SelectedPath;
			}
		}
		if (ImGui::IsKeyPressed(ImGuiKey_C) && !m_SelectedPaths.empty())
		{
			m_ClipboardPaths.assign(m_SelectedPaths.begin(), m_SelectedPaths.end());
			m_ClipboardMode = ClipboardMode::Copy;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_X) && !m_SelectedPaths.empty())
		{
			m_ClipboardPaths.assign(m_SelectedPaths.begin(), m_SelectedPaths.end());
			m_ClipboardMode = ClipboardMode::Cut;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_V) && !m_ClipboardPaths.empty())
		{
			PasteTo(m_CurrentDirectory);
		}
	}

	int ContentBrowserPanel::GetThumbnailBucket() const
	{
		// Deterministic fixed-size buckets to avoid texture churn while resizing the grid.
		if (m_thumbnailSize <= 48) return 32;
		if (m_thumbnailSize <= 96) return 64;
		return 128;
	}

	bool ContentBrowserPanel::IsThumbnailCandidate(const std::filesystem::path& path) const
	{
		std::string ext = path.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return (char)std::tolower(c); });
		return ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".tga" || ext == ".bmp" || ext == ".hdr";
	}

	Ref<Texture2D> ContentBrowserPanel::GetThumbnailTexture(const std::filesystem::path& path, int bucket)
	{
		std::string key = path.string() + "|" + std::to_string(bucket);
		auto it = m_ThumbnailCache.find(key);
		if (it != m_ThumbnailCache.end())
		{
			it->second.LastUsed = ++m_ThumbnailUseCounter;
			return it->second.Texture;
		}

		int srcW = 0, srcH = 0, srcChannels = 0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* src = stbi_load(path.string().c_str(), &srcW, &srcH, &srcChannels, 3);
		if (!src || srcW <= 0 || srcH <= 0)
		{
			if (src) stbi_image_free(src);
			return nullptr;
		}

		std::vector<unsigned char> dst((size_t)bucket * (size_t)bucket * 3);
		for (int y = 0; y < bucket; y++)
		{
			for (int x = 0; x < bucket; x++)
			{
				int sx = (x * srcW) / bucket;
				int sy = (y * srcH) / bucket;
				size_t si = ((size_t)sy * (size_t)srcW + (size_t)sx) * 3;
				size_t di = ((size_t)y * (size_t)bucket + (size_t)x) * 3;
				dst[di + 0] = src[si + 0];
				dst[di + 1] = src[si + 1];
				dst[di + 2] = src[si + 2];
			}
		}
		stbi_image_free(src);

		Ref<Texture2D> thumb = Texture2D::Create((uint32_t)bucket, (uint32_t)bucket);
		thumb->SetData(dst.data(), (uint32_t)dst.size());

		ThumbnailEntry entry{};
		entry.Texture = thumb;
		entry.LastUsed = ++m_ThumbnailUseCounter;
		m_ThumbnailCache[key] = entry;

		if (m_ThumbnailCache.size() > m_MaxThumbnailCacheEntries)
		{
			auto evictIt = m_ThumbnailCache.begin();
			for (auto jt = m_ThumbnailCache.begin(); jt != m_ThumbnailCache.end(); ++jt)
			{
				if (jt->second.LastUsed < evictIt->second.LastUsed)
					evictIt = jt;
			}
			m_ThumbnailCache.erase(evictIt);
		}
		return thumb;
	}

	void ContentBrowserPanel::InvalidateThumbnail(const std::filesystem::path& path)
	{
		const std::string prefix = path.string() + "|";
		for (auto it = m_ThumbnailCache.begin(); it != m_ThumbnailCache.end();)
		{
			if (it->first.rfind(prefix, 0) == 0)
				it = m_ThumbnailCache.erase(it);
			else
				++it;
		}
	}

	void ContentBrowserPanel::InvalidateThumbnail(const std::vector<std::filesystem::path>& paths)
	{
		for (const auto& p : paths)
			InvalidateThumbnail(p);
	}

}
