#include <MrgPch.h>

#include "Mirage/KeyCodes.h"
#include "Mirage/Input.h"
#include "OrthographicCameraController.h"

namespace Mirage
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
          m_AspectRatio(aspectRatio),
          m_Rotation(rotation)
    {
    }

    void OrthographicCameraController::OnUpdate(float DeltaTime)
    {
        if (Input::IsKeyPressed(MRG_Key_A))
        {
            m_CameraPosition.x -= (m_CameraTranslationSpeed * DeltaTime);
        }
        else if (Input::IsKeyPressed(MRG_Key_D))
        {
            m_CameraPosition.x += (m_CameraTranslationSpeed * DeltaTime);
        }
        if (Input::IsKeyPressed(MRG_Key_W))
        {
            m_CameraPosition.y += (m_CameraTranslationSpeed * DeltaTime);
        }
        else if (Input::IsKeyPressed(MRG_Key_S))
        {
            m_CameraPosition.y -= (m_CameraTranslationSpeed * DeltaTime);
        }

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(MRG_Key_Q))
            {
                m_CameraRotation.z += (m_CameraRotationSpeed * DeltaTime);
            }
            else if (Input::IsKeyPressed(MRG_Key_E))
            {
                m_CameraRotation.z -= (m_CameraRotationSpeed * DeltaTime);
            }
            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);
    }

    void OrthographicCameraController::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrollEvent>(MRG_BIND_EVENT_FN(&OrthographicCameraController::OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(MRG_BIND_EVENT_FN(&OrthographicCameraController::OnWindowResize));
    }

    bool OrthographicCameraController::OnMouseScroll(MouseScrollEvent& event)
    {
        m_ZoomLevel -= event.GetYOffset() * 0.1f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_ZoomLevel = std::min(m_ZoomLevel, 4.5f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        m_CameraTranslationSpeed = m_ZoomLevel;
        return false;
    }

    bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& event)
    {
        m_AspectRatio = event.GetWindowBounds().x / event.GetWindowBounds().y;
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
}
