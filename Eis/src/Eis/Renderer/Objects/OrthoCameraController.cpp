#include "Eispch.h"
#include "OrthoCameraController.h"

#include "Eis/Input/Input.h"
#include "Eis/Input/Keycodes.h"

namespace Eis
{
	OrthoCameraController::OrthoCameraController(float aspectRatio, bool movement, bool zoom, bool rotation)
		: m_AspectRatio(aspectRatio), m_lockMovement(movement), m_lockZoom(zoom), m_lockRotation(rotation),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void OrthoCameraController::OnUpdate(TimeStep ts)
	{
		
		if (!m_lockMovement)
		{
			if (Input::IsKeyPressed(EIS_KEY_UP) || Input::IsKeyPressed(EIS_KEY_W))
			{
				m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_MovementSpeed * ts;
				m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_MovementSpeed * ts;
			}
			if (Input::IsKeyPressed(EIS_KEY_DOWN) || Input::IsKeyPressed(EIS_KEY_S))
			{
				m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_MovementSpeed * ts;
				m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_MovementSpeed * ts;
			}
			if (Input::IsKeyPressed(EIS_KEY_LEFT) || Input::IsKeyPressed(EIS_KEY_A))
			{
				m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_MovementSpeed * ts;
				m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_MovementSpeed * ts;
			}
			if (Input::IsKeyPressed(EIS_KEY_RIGHT) || Input::IsKeyPressed(EIS_KEY_D))
			{
				m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_MovementSpeed * ts;
				m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_MovementSpeed * ts;
			}
		}
		if (!m_lockRotation)
		{
			if (Input::IsKeyPressed(EIS_KEY_Q))
				m_CameraRotation += m_RotationSpeed * ts;
			if (Input::IsKeyPressed(EIS_KEY_E))
				m_CameraRotation -= m_RotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}


		m_Camera.SetPosition(m_CameraPosition);

		m_MovementSpeed = m_ZoomLevel; // HACK
	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>(EIS_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(EIS_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent e)
	{
		if (m_lockZoom)
			return false;

		float last = m_ZoomLevel; // HACK: find better way to mantain maximum zoom level acording to sensitivity
		m_ZoomLevel -= e.GetYOffset() / m_ZoomSensitivity;

		if (m_ZoomLevel <= 0.2f)
			m_ZoomLevel = last;

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	
	}

	bool OrthoCameraController::OnWindowResized(WindowResizeEvent e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}