﻿#pragma once

#include "Mirage/Renderer/Camera.h"

namespace Mirage
{
    class SceneCamera : public Camera
    {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float size, float nearClip, float farClip);
        void SetPerspective(float vFov, float nearClip, float farClip);
        
        void SetViewportSize(uint32_t width, uint32_t height);
        
        float GetPerspectiveVFOV() const { return m_PerspectiveFOV; }
        void SetPerspectiveVFOV(float vFov) { m_PerspectiveFOV = vFov; RecalculateProjection(); }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
        void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

        float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }
        void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }
        
        ProjectionType GetProjectionType() const { return m_ProjectionType; }
        void SetProjectionType(int type) { m_ProjectionType = (ProjectionType)type; RecalculateProjection(); }

    	float GetAsectRatio() { return m_AspectRatio; }
        
    private:
        void RecalculateProjection();
        
    private:
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;
        
        float m_PerspectiveFOV = Radians(80.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
        
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 100.0f;

        float m_AspectRatio = 16.0f/9.0f;
    };
    
}
