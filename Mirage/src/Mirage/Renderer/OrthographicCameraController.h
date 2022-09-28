#pragma once

#include "Mirage/Renderer/OrthographicCamera.h"

#include "Mirage/Events/ApplicationEvent.h"
#include "Mirage/Events/MouseEvent.h"

namespace Mirage
{
    struct OrthographicCameraBounds
    {
        float left, right, bottom, top;

        float GetWidth() { return right - left; }
        float GetHeight() { return top - bottom; }
    };
    
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);

        void OnUpdate(float DeltaTime);
        void OnEvent(Event& event);

        const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
        
        inline OrthographicCamera& GetCamera() { return m_Camera; }
        inline const OrthographicCamera& GetCamera() const { return m_Camera; }

    private:
        bool OnMouseScroll(MouseScrollEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);

        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;

        OrthographicCameraBounds m_Bounds;

        bool m_Rotation;

        Vec3 m_CameraPosition{0.0f};
        Vec3 m_CameraRotation{0.0f};

        float m_CameraInitialTranslationSpeed = 1.0f;
        float m_CameraTranslationSpeed = 1.0f;
        float m_CameraRotationSpeed = 90.0f;
    };
}
