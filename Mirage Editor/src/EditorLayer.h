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
        OrthographicCameraController m_CameraController;
    
        Renderer2D::Primitives::Quad quad;
        Renderer2D::Primitives::Quad quad2;
        Ref<Framebuffer> m_Framebuffer;

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
    
        Vec3 m_Position = { 0.0f, 0.0f, -0.01f };
        Vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
        Vec3 m_Scale = { 20.0f, 20.0f, 1.0f };

        Ref<Texture2D> m_texture;
    
        Vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    
        Vec2 m_Tiling = { 20.0f, 20.0f };
        Vec2 m_Offset = { 0.0f, 0.0f };

        Vec2 m_ViewportSize = {0.0f, 0.0f};
        
    };

}
