#include "MrgPch.h"
#include "EditorCamera.h"

#include "Mirage/Core/Input.h"
#include "Mirage/Core/KeyCodes.h"
#include "Mirage/Core/MouseCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Mirage {
#if 0
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(Radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(Radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		Quat orientation = GetOrientation();
		m_ViewMatrix = MatTranslate(Mat4(1.0f), m_Position) * ToMat4(orientation);
		m_ViewMatrix = Inverse(m_ViewMatrix);
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float speed = 1 / (1 + glm::exp(- 0.5f * m_Distance + 4.0f));
		return speed * zoomSpeed;
	}

	void EditorCamera::OnUpdate(float deltaTime)
	{
		const Vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		Vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			MouseRotate(delta);
			if(m_Zoom != 0.0f)
			{
				Zoom(m_Zoom);	
			}
			if(m_Pan != Vec2(0.0f))
			{
				Pan(m_Pan);
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
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			if (e.GetKeyCode() == Key::W)
				m_Zoom = 1.0f;
			if (e.GetKeyCode() == Key::S)
				m_Zoom = -1.0f;
			if (e.GetKeyCode() == Key::A)
				m_Pan.x = -1.0f;
			if (e.GetKeyCode() == Key::D)
				m_Pan.x = 1.0f;
			if (e.GetKeyCode() == Key::Q)
				m_Pan.y = -1.0f;
			if (e.GetKeyCode() == Key::E)
				m_Pan.y = 1.0f;
			
			return true;
		}
		return false;
	}

	bool EditorCamera::OnKeyReleased(KeyReleasedEvent& e)
	{
		if (e.GetKeyCode() == Key::W || e.GetKeyCode() == Key::S)
			m_Zoom = 0.0f;
		if (e.GetKeyCode() == Key::A || e.GetKeyCode() == Key::D)
			m_Pan.x = 0.0f;
		if (e.GetKeyCode() == Key::Q || e.GetKeyCode() == Key::E)
			m_Pan.y = 0.0f;
		return false;
	}

	bool EditorCamera::OnMouseScroll(MouseScrollEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		Zoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::Pan(const Vec2& delta)
	{
		// Vec2 speed = PanSpeed();
		m_FocalPoint += GetRightDirection() * delta.x * panSpeed;
		m_FocalPoint += GetUpDirection() * delta.y * panSpeed;
	}

	void EditorCamera::MouseRotate(const Vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::Zoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		m_Distance = std::max(m_Distance, 0.1f);
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

	Vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	Quat EditorCamera::GetOrientation() const
	{
		return Quat(Vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
#else
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(Radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(Radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		Quat orientation = GetOrientation();
		m_ViewMatrix = MatTranslate(Mat4(1.0f), m_Position) * ToMat4(orientation);
		m_ViewMatrix = Inverse(m_ViewMatrix);
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed()
	{
		float speed =  1 / (1 + glm::exp(-m_Zooming + 4.0f));
		return speed * m_ZoomSpeed;
	}

	void EditorCamera::OnUpdate(float deltaTime)
	{
		const Vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		Vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			MouseRotate(delta);
			if(m_Pan != Vec3(0.0f))
			{
				bool isShiftClicked = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
				Move(m_Pan * deltaTime * (isShiftClicked? m_ShiftMultiplier : 1.0f));
			}
			UpdateView();
		}		 
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		if(e.IsInCategory(EventCategoryMouse))
			dispatcher.Dispatch<MouseScrollEvent>(MRG_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
		dispatcher.Dispatch<KeyPressedEvent>(MRG_BIND_EVENT_FN(EditorCamera::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(MRG_BIND_EVENT_FN(EditorCamera::OnKeyReleased));
		dispatcher.Dispatch<MouseScrollEvent>(MRG_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnKeyPressed(KeyPressedEvent& e)
	{
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			if (e.GetKeyCode() == Key::W)
				m_Pan.z = 1.0f;
			if (e.GetKeyCode() == Key::S)
				m_Pan.z = -1.0f;
			if (e.GetKeyCode() == Key::A)
				m_Pan.x = -1.0f;
			if (e.GetKeyCode() == Key::D)
				m_Pan.x = 1.0f;
			if (e.GetKeyCode() == Key::Q)
				m_Pan.y = -1.0f;
			if (e.GetKeyCode() == Key::E)
				m_Pan.y = 1.0f;
			
			return true;
		}
		return false;
	}

	bool EditorCamera::OnKeyReleased(KeyReleasedEvent& e)
	{
		if (e.GetKeyCode() == Key::W || e.GetKeyCode() == Key::S)
			m_Pan.z = 0.0f;
		if (e.GetKeyCode() == Key::A || e.GetKeyCode() == Key::D)
			m_Pan.x = 0.0f;
		if (e.GetKeyCode() == Key::Q || e.GetKeyCode() == Key::E)
			m_Pan.y = 0.0f;
		return false;
	}

	bool EditorCamera::OnMouseScroll(MouseScrollEvent& e)
	{
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			m_MoveSpeed += e.GetYOffset() * 0.1f;
		}
		else
		{
			float delta = e.GetYOffset();
			Zoom(delta);
			UpdateView();
			return true;
		}
	}

	void EditorCamera::Move(const Vec3& delta)
	{
		// Vec2 speed = PanSpeed();
		m_Position += GetForwardDirection() *  delta.z * m_MoveSpeed;
		m_Position += GetRightDirection() * delta.x * m_MoveSpeed;
		m_Position += GetUpDirection() * delta.y * m_MoveSpeed;
	}

	void EditorCamera::MouseRotate(const Vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::Zoom(float delta)
	{
		m_Zooming -= delta * ZoomSpeed();
		m_Position += GetForwardDirection() * delta * ZoomSpeed();
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

	Vec3 EditorCamera::CalculatePosition() const
	{
		return m_Position;
	}

	Quat EditorCamera::GetOrientation() const
	{
		return Quat(Vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
#endif
}
