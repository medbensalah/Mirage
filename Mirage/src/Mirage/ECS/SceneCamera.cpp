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
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
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
        float orthoLeft = -0.5f * m_OrthographicSize * m_AspectRatio;
        float orthoRight = -orthoLeft;
        float orthoBottom = -0.5f * m_OrthographicSize;
        float orthoTop = -orthoBottom;

        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
    }
}
