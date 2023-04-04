#pragma once

#include "Mirage.h"
#include "ImGui/imgui_internal.h"
#include "Mirage/Events/KeyEvent.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Mirage/ECS/SceneCamera.h"
#include "Panels/Extras/SettingsPanel.h"
#include "Panels/Extras/GraphEditor/GraphEditor.h"

namespace Mirage
{	
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(float DeltaTime) override;
        virtual void OnImGuiRender() override;
        void OnEvent(Event& e) override;

    private:    	
        bool OnShortcutKeyPressed(KeyPressedEvent e);
        
		bool CanPick();
    	bool IsInViewportSpace(Vec2 pos);
    	VecI2 GetMouseViewportSpace();
    	Vec3 GetMouseWorldSpace();
    	int GetIDat(Vec2 MousePos);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

    	void OnOverlayRender();
        
        void CreateDockspace();
        void CreateMenuBar();
        void CreateViewport();
        void CreateGamePreview();
        void CreateToolBar();


    	void DuplicateSelected();

    	
        void NewScene();
        void OpenScene();
        void OpenScene(const std::filesystem::path& path);
        void Save();
        void SaveAs();


    	void OnScenePlay();
    	void OnSceneStop();

    	void OnSceneSimulate();
    	
    private:
        Ref<Framebuffer> m_Framebuffer;
        Ref<Framebuffer> m_PreviewFramebuffer;

    	bool m_ShowStats = false;
    	bool m_ShowPreview = false;
    	
        Ref<Scene> m_ActiveScene;
#define COPY_RUNTIME_SCENE 0
#if COPY_RUNTIME_SCENE
        Ref<Scene> m_EditorScene;
#endif
		std::filesystem::path m_ActiveScenePath;
        
        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;

    	bool m_ViewportVisible = true;
    	bool m_GamePreviewVisible = true;

        EditorCamera m_EditorCamera;
    	SceneCamera m_SceneCamera;
    
        Ref<Texture2D> m_texture;

        Vec2 m_ViewportSize = {0.0f, 0.0f};
    	Vec2 m_PreviewSize = {420.0f, 236.0f};
        Vec2 m_ViewportBounds[2];

    	// ---------------------- Overlays ----------------------
    	bool m_ShowPhysicsColliders = false;
        
        // ----------------------- Gizmos -----------------------
        ImGuizmo::OPERATION m_GizmoType = (ImGuizmo::OPERATION)-1;
        ImGuizmo::MODE m_GizmoMode = ImGuizmo::MODE::WORLD;

    	bool m_ShowGrid = true;
    	float m_GridSize = 100.0f;
    	float m_GridAlpha = 1.0f;
        
        // ----------------------- Snapping -----------------------
        bool m_TranslationSnap = false;
        bool m_RotationSnap = false;
        bool m_ScaleSnap = false;
        
        float m_TranslationSnapValue = 0.5f;
        float m_RotationSnapValue = 15.0f;
        float m_ScaleSnapValue = 0.25f;
        
        // ----------------------- Panels -----------------------
        HierarchyPanel m_HierarchyPanel;
        ContentBrowserPanel m_ContentBrowserPanel;

    	// ----------------------- Extras -----------------------
    	bool m_ShowSettings = false;
        SettingsPanel m_SettingsPanel;
    	bool m_ShowGraphEditor = false;
    	Graph::GraphEditor m_GraphEditor;
    	
    	// ----------------------- Icons -----------------------

    	float m_IconSizeS = 20.0f;
    	Ref<Texture2D> m_WorldSpaceIcon;
    	Ref<Texture2D> m_LocalSpaceIcon;
    	
    	Ref<Texture2D> m_TranslationIcon;
    	Ref<Texture2D> m_RotationIcon;
    	Ref<Texture2D> m_ScaleIcon;
    	
    	enum class SceneState
    	{
    		Edit = 0, 
			Play,
    		Simulate
		};

    	SceneState m_SceneState = SceneState::Edit;
    	
    	Ref<Texture2D> m_PlayButtonIcon;
    	Ref<Texture2D> m_SimulateButtonIcon;
    	float m_PlayButtonIconSize = 25.0f;
    	
    	Ref<Texture2D> m_PauseButtonIcon;
    	Ref<Texture2D> m_StopButtonIcon;
    };

}
