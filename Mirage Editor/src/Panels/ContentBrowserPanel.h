#pragma once

#include <filesystem>
#include <optional>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>

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
		void OnExternalFilesDropped(const std::vector<std::string>& paths);

	private:
		void DrawHeader();
		void DrawDirectoryHierarchy();
		void DrawContents();
		void DrawStatusBar();
		void DrawContextMenu();
		void HandleShortcuts();
		int GetThumbnailBucket() const;
		bool IsThumbnailCandidate(const std::filesystem::path& path) const;
		Ref<Texture2D> GetThumbnailTexture(const std::filesystem::path& path, int bucket);
		void InvalidateThumbnail(const std::filesystem::path& path);
		void InvalidateThumbnail(const std::vector<std::filesystem::path>& paths);

		Ref<Texture2D> SetIcon(std::filesystem::path path);
		
		void DrawDirectoryNode(std::filesystem::path path, const char* filename);

		void MarkUpdateContents() { m_MarkUpdateContents = true; }
		void MarkUpdateTree() { m_MarkUpdateTree = true; }
		void UpdateBrowser();
		std::vector<std::filesystem::path> Browse(const std::filesystem::path& path);
		
		void NavigateBackward();
		void NavigateForward();

		void OnFileDoubleClick(std::filesystem::path path);
		void QueueDeleteSelection();
		void PasteTo(const std::filesystem::path& targetDirectory);
		std::filesystem::path MakeUniquePath(const std::filesystem::path& candidate) const;
		void RemovePath(const std::filesystem::path& path);
		void SelectSingle(const std::filesystem::path& path);
		void SelectRangeTo(const std::filesystem::path& path);
		bool IsSelected(const std::filesystem::path& path) const;
		void BeginRename(const std::filesystem::path& path);
		void OpenInFileExplorer(const std::filesystem::path& path);
		
		// --------------------------- Drag & Drop ---------------------------
		void SetPayload();
		void SetTooltip();

		// --------------------------- Double clicking file ---------------------------
		bool m_IsSceneRequested = false;
		std::filesystem::path m_RequestedScenePath;
		
	private:
		std::filesystem::path m_CurrentDirectory;
		std::filesystem::path m_SelectedPath;
		std::unordered_set<std::filesystem::path> m_SelectedPaths;
		std::vector<std::filesystem::path> m_ClipboardPaths;
		enum class ClipboardMode { None = 0, Copy, Cut };
		ClipboardMode m_ClipboardMode = ClipboardMode::None;
		std::vector<std::filesystem::path> m_DeletePending;
		int m_NewFolderCounter = 1;
		std::filesystem::path m_RenameTarget;
		char m_RenameBuffer[260] = {};
		std::filesystem::path m_SelectionAnchor;
		bool m_RenameFocusPending = false;

		struct ThumbnailEntry
		{
			Ref<Texture2D> Texture;
			uint64_t LastUsed = 0;
		};
		std::unordered_map<std::string, ThumbnailEntry> m_ThumbnailCache;
		uint64_t m_ThumbnailUseCounter = 0;
		size_t m_MaxThumbnailCacheEntries = 256;

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
