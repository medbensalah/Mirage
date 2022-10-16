#include <MrgPch.h>

#include "SceneCamera.h"

namespace Mirage
{
    SceneCamera::SceneCamera()
    {
        RecalculateProjection();
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Orthographic;
        
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
        
        RecalculateProjection();
    }

    void SceneCamera::SetPerspective(float vFov, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Perspective;
        
        m_PerspectiveFOV = vFov;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar = farClip;
        
        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        float aspectRatio = (float)width / (float)height;
        if(aspectRatio != m_AspectRatio)
        {
            m_AspectRatio = aspectRatio;
            RecalculateProjection();
        }
    }

    void SceneCamera::RecalculateProjection()
    {
        switch (m_ProjectionType)
        {
        case Perspective:
            m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
            break;
        case Orthographic:
            float orthoLeft = -0.5f * m_OrthographicSize * m_AspectRatio;
            float orthoRight = -orthoLeft;
            float orthoBottom = -0.5f * m_OrthographicSize;
            float orthoTop = -orthoBottom;

            m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
            break;
        }
    }
}
