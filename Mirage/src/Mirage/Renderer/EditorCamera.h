#pragma once

#include "Camera.h"
#include "Mirage/Events/Event.h"
#include "Mirage/Events/MouseEvent.h"

#include <glm/glm.hpp>

#include "Mirage/Events/KeyEvent.h"

namespace Mirage {

    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate(float deltaTime);
        void OnEvent(Event& e);

        inline float GetDistance() const { return m_Zooming; }
        inline void SetDistance(float distance) { m_Zooming = distance; }

        inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

        const Mat4& GetViewMatrix() const { return m_ViewMatrix; }
        Mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

        Vec3 GetRightDirection() const;
        Vec3 GetUpDirection() const;
        Vec3 GetForwardDirection() const;
        const Vec3& GetPosition() const { return m_Position; }
        Quat GetOrientation() const;

        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }

    private:
        void UpdateProjection();
        void UpdateView();

        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
        bool OnMouseScroll(MouseScrollEvent& e);

        void Move(const Vec3& delta);
        void MouseRotate(const Vec2& delta);
        void Zoom(float delta);

        Vec3 CalculatePosition() const;

        float RotationSpeed() const;
        float ZoomSpeed();
    private:
        float m_FOV = 60.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

        Mat4 m_ViewMatrix;
        Vec3 m_Position = { 0.0f, 0.0f, 10.0f };

        Vec2 m_InitialMousePosition = { 0.0f, 0.0f };

        float m_MoveSpeed = 2.0f;
        float m_ShiftMultiplier = 3.0f;
        float m_Zooming = 10.0f;
        float m_ZoomSpeed = 0.5f;
        float m_Pitch = 0.0f, m_Yaw = 0.0f;

        float m_ViewportWidth = 1280, m_ViewportHeight = 720;

        Vec3 m_Pan{0.0f};


        friend class EditorLayer;
    };

}
