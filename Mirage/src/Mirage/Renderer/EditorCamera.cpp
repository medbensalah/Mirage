#include "MrgPch.h"
#include "EditorCamera.h"

#include "Mirage/Core/Input.h"
#include "Mirage/Core/KeyCodes.h"
#include "Mirage/Core/MouseCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Mirage {
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_ProjectionType(ProjectionType::Perspective),
		  m_PerspectiveData(PerspectiveData{fov, nearClip, farClip, Vec3{1.5f, 1.25f, 1.5f}, 10.0f}),
		  m_OrthographicData(OrthographicData{10.0f, -1.0f, farClip, Vec3{0.0f, 0.0f, 10.0f}, 10.0f}),
	      m_AspectRatio(aspectRatio)
	{
		UpdateView();
	}

	void EditorCamera::SetProjectionType(ProjectionType projectionType)
	{
		m_ProjectionType = projectionType;
        UpdateProjection();
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		switch (m_ProjectionType)
		{
		case ProjectionType::Perspective:
			m_Projection = glm::perspective(Radians(m_PerspectiveData.m_FOV), m_AspectRatio, m_PerspectiveData.m_NearClip, m_PerspectiveData.m_FarClip);
			break;
		case ProjectionType::Orthographic:
			float left = -0.5f * m_OrthographicData.m_OrthographicSize * m_AspectRatio;
			float right = -left;
			float bottom = -0.5f * m_OrthographicData.m_OrthographicSize;
			float top = -bottom;
			
			m_Projection = glm::ortho(left, right, bottom, top, m_OrthographicData.m_OrthographicNear, m_OrthographicData.m_OrthographicFar);
		}
	}

	void EditorCamera::UpdateView()
	{
		Quat orientation = GetOrientation();
		m_ViewMatrix = MatTranslate(Mat4(1.0f), GetPosition()) * ToMat4(orientation);
		m_ViewMatrix = Inverse(m_ViewMatrix);
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed()
	{
		if(m_ProjectionType == ProjectionType::Orthographic)
			return m_ZoomSpeed;
		else
		{
			float speed =  1 / (1 + glm::exp(-m_PerspectiveData.m_Zooming + 4.0f));
			return speed * m_ZoomSpeed;
		}
	}

	void EditorCamera::OnUpdate(float deltaTime)
	{
		const Vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		Vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			if (delta != Vec2(0.0f))
			{
				MouseRotate(delta);
			}
			if (m_PanDelta != Vec3(0.0f))
			{
				bool isShiftClicked = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
				Move(m_PanDelta * deltaTime * (isShiftClicked? m_ShiftMultiplier : 1.0f));
			}
			UpdateView();
		}		 
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(MRG_BIND_EVENT_FN(EditorCamera::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(MRG_BIND_EVENT_FN(EditorCamera::OnKeyReleased));
		dispatcher.Dispatch<MouseScrollEvent>(MRG_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::D2)
			SetProjectionType(m_ProjectionType == ProjectionType::Perspective
				                  ? ProjectionType::Orthographic
				                  : ProjectionType::Perspective);
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			switch (m_ProjectionType)
			{
			case Orthographic:
				if (e.GetKeyCode() == Key::W)
					m_PanDelta.y = 1.0f;
				if (e.GetKeyCode() == Key::S)
					m_PanDelta.y = -1.0f;
				if (e.GetKeyCode() == Key::A)
					m_PanDelta.x = -1.0f;
				if (e.GetKeyCode() == Key::D)
					m_PanDelta.x = 1.0f;
			
				return true;
			case Perspective:
				if (e.GetKeyCode() == Key::W)
					m_PanDelta.z = 1.0f;
				if (e.GetKeyCode() == Key::S)
					m_PanDelta.z = -1.0f;
				if (e.GetKeyCode() == Key::A)
					m_PanDelta.x = -1.0f;
				if (e.GetKeyCode() == Key::D)
					m_PanDelta.x = 1.0f;
				if (e.GetKeyCode() == Key::Q)
					m_PanDelta.y = -1.0f;
				if (e.GetKeyCode() == Key::E)
					m_PanDelta.y = 1.0f;
			
				return true;
			}
		}
		return false;
	}

	bool EditorCamera::OnKeyReleased(KeyReleasedEvent& e)
	{
		switch (m_ProjectionType)
		{
		case Orthographic:
			if (e.GetKeyCode() == Key::W || e.GetKeyCode() == Key::S)
			{
				m_PanDelta.y = 0.0f; m_PanDelta.z = 0.0f;
			}
			if (e.GetKeyCode() == Key::A || e.GetKeyCode() == Key::D)
				m_PanDelta.x = 0.0f;
			return true;
		case Perspective:
			if (e.GetKeyCode() == Key::W || e.GetKeyCode() == Key::S)
				m_PanDelta.z = 0.0f;
			if (e.GetKeyCode() == Key::A || e.GetKeyCode() == Key::D)
				m_PanDelta.x = 0.0f;
			if (e.GetKeyCode() == Key::Q || e.GetKeyCode() == Key::E)
				m_PanDelta.y = 0.0f;
			return true;
		}
		return false;
	}

	bool EditorCamera::OnMouseScroll(MouseScrollEvent& e)
	{
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			m_MoveSpeed += e.GetYOffset() * 0.1f;
			m_MoveSpeed = std::max(m_MoveSpeed, 0.01f);
		}
		else
		{
			float delta = e.GetYOffset();
			Zoom(delta);
		}
		return true;
	}

	void EditorCamera::Move(const Vec3& delta)
	{
		if(m_ProjectionType == ProjectionType::Orthographic)
		{
			m_OrthographicData.m_Position += GetRightDirection() * delta.x * m_MoveSpeed;
			m_OrthographicData.m_Position += GetForwardDirection() *  delta.z * m_MoveSpeed;
			m_OrthographicData.m_Position += GetUpDirection() * delta.y * m_MoveSpeed;
		}
		else
		{
			m_PerspectiveData.m_Position += GetForwardDirection() *  delta.z * m_MoveSpeed;
			m_PerspectiveData.m_Position += GetRightDirection() * delta.x * m_MoveSpeed;
			m_PerspectiveData.m_Position += GetUpDirection() * delta.y * m_MoveSpeed;
		}
	}

	void EditorCamera::MouseRotate(const Vec2& delta)
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
			m_Yaw += yawSign * delta.x * RotationSpeed();
			m_Pitch += delta.y * RotationSpeed();
		}
	}

	void EditorCamera::Zoom(float delta)
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			m_OrthographicData.m_OrthographicSize -= delta * ZoomSpeed();
			m_OrthographicData.m_OrthographicSize = std::max(m_OrthographicData.m_OrthographicSize, 1.0f);
			UpdateProjection();
		}
		else
		{
			m_PerspectiveData.m_Zooming -= delta * ZoomSpeed();
			m_PerspectiveData.m_Position += GetForwardDirection() * delta * ZoomSpeed();
		}
		UpdateView();
	}

	Vec3 EditorCamera::GetUpDirection() const
	{
		return MatRotate(GetOrientation(), Vec3(0.0f, 1.0f, 0.0f));
	}

	Vec3 EditorCamera::GetRightDirection() const
	{
		return MatRotate(GetOrientation(), Vec3(1.0f, 0.0f, 0.0f));
	}

	Vec3 EditorCamera::GetForwardDirection() const
	{
		return MatRotate(GetOrientation(), Vec3(0.0f, 0.0f, -1.0f));
	}

	Quat EditorCamera::GetOrientation() const
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
		{
			return Quat(Vec3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			return Quat(Vec3(-m_Pitch, -m_Yaw, 0.0f));
		}
	}
}
