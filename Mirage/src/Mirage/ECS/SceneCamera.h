#pragma once

#include "Mirage/Renderer/Camera.h"

namespace Mirage
{
    class SceneCamera : public Camera
    {
    public:
        enum ProjectionType { Perspective = 0, Orthographic = 1 };
        
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float size, float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);

        float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }
        void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
        void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }
        
        ProjectionType GetProjectionType() const { return m_ProjectionType; }
        void SetProjectionType(int type) { m_ProjectionType = (ProjectionType)type; RecalculateProjection(); }
        
    private:
        void RecalculateProjection();
        
    private:
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;
        
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

        float m_AspectRatio = 0.0f;
    };
    
}
