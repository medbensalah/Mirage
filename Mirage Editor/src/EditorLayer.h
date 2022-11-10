#pragma once

#include "Mirage.h"
#include "ImGui/imgui_internal.h"
#include "Mirage/Events/KeyEvent.h"
#include "Panels/HierarchyPanel.h"
#include "ImGuizmo/ImGuizmo.h"

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

        void CreateDockspace();
        void CreateMenuBar();
        void CreateViewport();
        void CreateToolBar();

    private:
        void ProcessShortcuts(Event& e);
        bool OnShortcutKeyPressed(KeyPressedEvent e);

        void NewScene();
        void OpenScene();
        void SaveAs();
        
    private:
        Ref<Framebuffer> m_Framebuffer;
        
        Ref<Scene> m_ActiveScene;
        OrthographicCameraController m_CameraController;

        SceneObject m_SquareEntity;
        SceneObject m_Camera;
        
        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
    
        Ref<Texture2D> m_texture;

        Vec2 m_ViewportSize = {0.0f, 0.0f};

        
        // ----------------------- Gizmos -----------------------
        ImGuizmo::OPERATION m_GizmoType = (ImGuizmo::OPERATION)-1;
        ImGuizmo::MODE m_GizmoMode = ImGuizmo::MODE::WORLD;
        
        // Snapping
        bool m_TranslationSnap = false;
        bool m_RotationSnap = false;
        bool m_ScaleSnap = false;
        
        float m_TranslationSnapValue = 0.5f;
        float m_RotationSnapValue = 15.0f;
        float m_ScaleSnapValue = 0.25f;
        
        // ----------------------- Panels -----------------------
        HierarchyPanel m_HierarchyPanel;
    };

}
