#include <MrgPch.h>

#include "OrthographicCamera.h"


namespace Mirage
{
    OrthographicCamera::OrthographicCamera(
        float left, float right,
        float bottom, float top,
        float nearPlane,
        float farPlane
    )
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, nearPlane, farPlane)), m_ViewMatrix(1.0f)
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right,
        float bottom, float top,
        float nearPlane, float farPlane
        )
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        Mat4 transform = MatTranslate(Mat4(1.0f), m_Position)
            * MatRotate(Mat4(1.0f), Radians(m_Rotation.x),Vec3(1.0f, 0.0f, 0.0f))
            * MatRotate(Mat4(1.0f), Radians(m_Rotation.y),Vec3(0.0f, 1.0f, 0.0f))
            * MatRotate(Mat4(1.0f), Radians(m_Rotation.z),Vec3(0.0f, 0.0f, 1.0f));

        m_ViewMatrix = Invert(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
