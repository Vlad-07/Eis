#include "Eispch.h"
#include "EditorCamera2.h"

#include "Eis/Input/Input.h"
#include "Eis/Core/Time.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/matrix_clip_space.hpp>


namespace Eis
{
	void EditorCamera2::OnUpdate()
	{
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			if (Input::IsKeyPressed(Key::W))
				m_Position += GetForwardDir() * m_Speed * (float)Time::GetDeltaTime();
			else if (Input::IsKeyPressed(Key::S))
				m_Position -= GetForwardDir() * m_Speed * (float)Time::GetDeltaTime();
			if (Input::IsKeyPressed(Key::D))
				m_Position += GetRightDir() * m_Speed * (float)Time::GetDeltaTime();
			else if (Input::IsKeyPressed(Key::A))
				m_Position -= GetRightDir() * m_Speed * (float)Time::GetDeltaTime();
			if (Input::IsKeyPressed(Key::E))
				m_Position += GetUpDir() * m_Speed * (float)Time::GetDeltaTime();
			else if (Input::IsKeyPressed(Key::Q))
				m_Position -= GetUpDir() * m_Speed * (float)Time::GetDeltaTime();

			// TODO: panning
			// TODO: 2D view

			const glm::vec2 mouse = Input::GetMousePos();
			const glm::vec2 delta = -(mouse - m_LastMousePos) * 0.003f;
			m_LastMousePos = mouse;

			m_Rotation.y += delta.x * m_Sensitivity;
			m_Rotation.x += delta.y * m_Sensitivity;
		}
		else
			m_LastMousePos = Input::GetMousePos(); // probably bad

		UpdateView();
	}

	void EditorCamera2::OnEvent(Event& event)
	{
		EventDispatcher d{ event };
		d.Dispatch<MouseScrolledEvent>(
			[&](MouseScrolledEvent& event)
			{
				m_Speed *= glm::pow(1.2f, event.GetYOffset());
				return false;
			}
		);
	}


	void EditorCamera2::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera2::UpdateView()
	{
		m_ViewMatrix = glm::translate(glm::mat4{ 1.0f }, m_Position) * glm::toMat4(glm::quat{ m_Rotation });
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}


	glm::vec3 EditorCamera2::GetUpDir()
	{
		return glm::quat{ m_Rotation } * glm::vec3{ 0, 1, 0 };
	}
	glm::vec3 EditorCamera2::GetForwardDir()
	{
		return glm::quat{ m_Rotation } *glm::vec3{ 0, 0, -1 };
	}
	glm::vec3 EditorCamera2::GetRightDir()
	{
		return glm::quat{ m_Rotation } *glm::vec3{ 1, 0, 0 };
	}
}