#pragma once

#include <filesystem>
#include <stack>
#include <unordered_map>

#include "ImGui/imgui.h"
#include "Mirage/Core/Core.h"
#include "Mirage/Renderer/Texture.h"


namespace Mirage
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
		void NavigateTo(const std::filesystem::path& path);

	private:
		void DrawHeader();
		void DrawDirectoryHierarchy();
		void DrawContents();
		void DrawStatusBar();

		Ref<Texture2D> SetIcon(std::filesystem::path path);
		
		void DrawDirectoryNode(std::filesystem::path path, const char* filename);

		void MarkUpdateContents() { m_MarkUpdateContents = true; }
		void MarkUpdateTree() { m_MarkUpdateTree = true; }
		void UpdateBrowser();
		std::vector<std::filesystem::path> Browse(const std::filesystem::path& path);
		
		void NavigateBackward();
		void NavigateForward();

		void OnFileDoubleClick(std::filesystem::path path);
		
		// --------------------------- Drag & Drop ---------------------------
		void SetPayload();
		void SetTooltip();

		// --------------------------- Double clicking file ---------------------------
		bool m_IsSceneRequested = false;
		std::filesystem::path m_RequestedScenePath;
		
	private:
		std::filesystem::path m_CurrentDirectory;
		std::filesystem::path m_SelectedPath;

		ImVec4 m_DirectoryTintColor = { 0.924f, 0.735f, 0.254f, 1.0f };
		ImVec4 m_FileTintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		ImVec4 m_SelectedBGColor = { 0.3f, 0.3f, 0.3f, 1.0f };
		
		std::vector<std::filesystem::path> m_Entries;			// All entries in the current directory
		std::unordered_map<ImGuiID, std::vector<std::filesystem::path>> m_BrowseCache;		// Directory hierarchy
		std::stack<std::filesystem::path> m_ForwardNavigation;
		std::stack<std::filesystem::path> m_BackWardNavigation;
		
		float m_LeftPanelSize = 200.0f;
		float m_RightPanelSize;
		float m_SeparatorThickness = 8.0f;
		
		int m_thumbnailSize = 64.0f;
		float m_itemSpacing = 8.0f;
		
		bool m_MarkUpdateContents = true;
		bool m_MarkUpdateTree = true;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_MirageIcon;
		
		friend class EditorLayer;
	};
}
