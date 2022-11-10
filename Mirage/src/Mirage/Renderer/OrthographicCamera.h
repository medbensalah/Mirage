#pragma once

#include "Mirage/Math/glmTypes.h"

namespace Mirage
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);
        void SetProjection(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);

        
        const Vec3& GetPosition() const { return m_Position; }
        void SetPosition(const Vec3& position)
        {
            m_Position = position;
            RecalculateViewMatrix();
        }

        const Vec3& GetRotation() const { return m_Rotation; }
        void SetRotation(const Vec3& Rotation)
        {
            m_Rotation = Rotation;
            RecalculateViewMatrix();
        }
        
        const Mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const Mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const Mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        
    private:
        void RecalculateViewMatrix();
        
        Mat4 m_ProjectionMatrix;
        Mat4 m_ViewMatrix;
        Mat4 m_ViewProjectionMatrix;

        Vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        Vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
    };
}
