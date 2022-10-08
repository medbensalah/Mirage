#pragma once

#include "Mirage.h"

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
        void CreateViewport();
        
    private:
        Ref<Framebuffer> m_Framebuffer;
        
        Ref<Scene> m_ActiveScene;
        OrthographicCameraController m_CameraController;

        SceneObject m_SquareEntity;

        

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
    
        Ref<Texture2D> m_texture;
    

        Vec2 m_ViewportSize = {0.0f, 0.0f};
        
    };

}
