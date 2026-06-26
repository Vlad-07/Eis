#include "Eispch.h"
#include "EditorCamera.h"

#include "Eis/Input/Input.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Eis
{
	EditorCamera::EditorCamera(float vFov, float aspectRatio, float nearClip, float farClip)
		: m_FOV{ vFov }, m_AspectRatio{ aspectRatio }, m_NearClip{ nearClip }, m_FarClip{ farClip },
		Camera{ glm::perspective(glm::radians(vFov), aspectRatio, nearClip, farClip) }
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_Position = CalculatePosition();

		const glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4{ 1.0f }, m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);

	}


	void EditorCamera::OnUpdate()
	{
		// TODO: unity control scheme?
		if (Input::IsKeyPressed(Key::LeftShift))
		{
			const glm::vec2 mouse = Input::GetMousePos();
			const glm::vec2 delta = (mouse - m_InitialMousePos) * 0.003f;
			m_InitialMousePos = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
	}



	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher d{ e };
		d.Dispatch<MouseScrolledEvent>(EIS_BIND_EVENT_FN(OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		const float delta = e.GetYOffset() * 0.1f;

		MouseZoom(delta);
		UpdateView();

		return false;
	}


	void EditorCamera::MousePan(glm::vec2 delta)
	{
		glm::vec2 speed = PanSpeed();
		m_FocalPoint += -GetRightDir() * delta.x * speed.x * m_Distance;
		m_FocalPoint += GetUpDir() * delta.y * speed.y * m_Distance;
	}

	void EditorCamera::MouseRotate(glm::vec2 delta)
	{
		const float yawSign = GetUpDir().y >= 0.0f ? 1.0f : -1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDir();
			m_Distance = 1.0f;
		}
	}



	glm::vec2 EditorCamera::PanSpeed() const
	{
		const float x = glm::min(m_ViewportWidth / 1000.0f, 2.4f);
		const float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
		
		const float y = glm::min(m_ViewportHeight/ 1000.0f, 2.4f);
		const float yFactor = 0.0366f * (y *y) - 0.1778f * y + 0.3021f;

		return glm::vec2{ xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		const float distance = glm::max(m_Distance * 0.2f, 0.0f);
		const float speed = glm::min(distance * distance, 100.0f);

		return speed;
	}


	glm::vec3 EditorCamera::GetUpDir() const
	{
		return glm::rotate(GetOrientation(), glm::vec3{ 0.0f, 1.0f, 0.0f });
	}
	glm::vec3 EditorCamera::GetRightDir() const
	{
		return glm::rotate(GetOrientation(), glm::vec3{ 1.0f, 0.0f, 0.0f });
	}
	glm::vec3 EditorCamera::GetForwardDir() const
	{
		return glm::rotate(GetOrientation(), glm::vec3{ 0.0f, 0.0f, -1.0f });
	}
	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDir() * m_Distance;
	}
	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat{ glm::vec3{ -m_Pitch, -m_Yaw, 0.0f } };
	}
}