#pragma once

#include "Mirage.h"
#include "Mirage/Events/KeyEvent.h"
#include "Panels/HierarchyPanel.h"

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

        //Panels
        HierarchyPanel m_HierarchyPanel;
    };

}
