#include <MrgPch.h>

#include "Mirage/Core/KeyCodes.h"
#include "Mirage/Core/Input.h"
#include "Mirage/Renderer/OrthographicCameraController.h"

namespace Mirage
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
          m_AspectRatio(aspectRatio),
          m_Rotation(rotation),
          m_Bounds({-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel})
    {
    }

    void OrthographicCameraController::OnUpdate(float DeltaTime)
    {
        MRG_PROFILE_FUNCTION();
        
        if (Input::IsKeyPressed(Key::A))
        {
            m_CameraPosition.x -= cos(Radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * DeltaTime;
            m_CameraPosition.y -= sin(Radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * DeltaTime;
        }
        else if (Input::IsKeyPressed(Key::D))
        {
            m_CameraPosition.x += cos(Radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * DeltaTime;
            m_CameraPosition.y += sin(Radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * DeltaTime;
        }
        if (Input::IsKeyPressed(Key::W))
        {
            m_CameraPosition.x += -sin(Radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * DeltaTime;
            m_CameraPosition.y += cos(Radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * DeltaTime;
        }
        else if (Input::IsKeyPressed(Key::S))
        {
            m_CameraPosition.x -= -sin(Radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * DeltaTime;
            m_CameraPosition.y -= cos(Radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * DeltaTime;
        }

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(Key::Q))
            {
                m_CameraRotation.z += (m_CameraRotationSpeed * DeltaTime);
            }
            else if (Input::IsKeyPressed(Key::E))
            {
                m_CameraRotation.z -= (m_CameraRotationSpeed * DeltaTime);
            }
            
            if (m_CameraRotation.z > 180.0f)
            {
                m_CameraRotation.z -= 360.0f;
            }
            else if (m_CameraRotation.z <= -180.0f)
            {
                m_CameraRotation.z += 360.0f;
            }
            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);
    }

    void OrthographicCameraController::OnEvent(Event& event)
    {
        MRG_PROFILE_FUNCTION();
        
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrollEvent>(MRG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(MRG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
    }

    void OrthographicCameraController::OnResize(float width, float height)
    {
        MRG_PROFILE_FUNCTION();
        
        m_AspectRatio = width / height;
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    bool OrthographicCameraController::OnMouseScroll(MouseScrollEvent& event)
    {
        MRG_PROFILE_FUNCTION();
        
        m_ZoomLevel -= event.GetYOffset() * 0.075f * m_ZoomLevel;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.15f);
        m_ZoomLevel = std::min(m_ZoomLevel, 9.0f);
        
        m_Bounds.left = -m_AspectRatio * m_ZoomLevel;
        m_Bounds.right = m_AspectRatio * m_ZoomLevel;
        m_Bounds.bottom = -m_ZoomLevel;
        m_Bounds.top = m_ZoomLevel;
        
        m_Camera.SetProjection(m_Bounds.left, m_Bounds.right, m_Bounds.bottom, m_Bounds.top);
        m_CameraTranslationSpeed = m_CameraInitialTranslationSpeed * glm::pow(m_ZoomLevel, 1.75f);
        m_CameraTranslationSpeed = std::max(m_CameraTranslationSpeed, 0.25f);
        m_CameraTranslationSpeed = std::min(m_CameraTranslationSpeed, 2.5f);
        return false;
    }

    bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& event)
    {
        MRG_PROFILE_FUNCTION();

        OnResize(event.GetWindowBounds().x, event.GetWindowBounds().y);
        
        return false;
    }
}
