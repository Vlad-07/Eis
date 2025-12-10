#include <Eispch.h>
#include "OrthoCameraController.h"

#include "Eis/Core/Application.h"
#include "Eis/Input/Input.h"


namespace Eis
{
	OrthoCameraController::OrthoCameraController(float aspectRatio)
	{
		// TODO: fixed aspect ratio
		// might be best handled by the window sistem and keep this as is (free aspect)
		if (aspectRatio != 0) EIS_CORE_WARN("Fixed aspect ratio not implemented!");

		m_AspectRatio = (float)Eis::Application::Get().GetWindow().GetWidth() / Eis::Application::Get().GetWindow().GetHeight();
		m_Camera = OrthographicCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthoCameraController::Update(TimeStep ts)
	{
		EIS_PROFILE_FUNCTION();

		if (m_PoseLock) return; // you kinda never use rotation, even less without position

		glm::vec2 delta(0.0f);
		const float sinRot = sin(glm::radians(m_Camera.GetRotation())),
					cosRot = cos(glm::radians(m_Camera.GetRotation()));
		if (Input::IsKeyPressed(EIS_KEY_W) || Input::IsKeyPressed(EIS_KEY_UP))
		{
			delta.x += -sinRot;
			delta.y +=  cosRot;
		}
		if (Input::IsKeyPressed(EIS_KEY_S) || Input::IsKeyPressed(EIS_KEY_DOWN))
		{
			delta.x -= -sinRot;
			delta.y -=  cosRot;
		}
		if (Input::IsKeyPressed(EIS_KEY_A) || Input::IsKeyPressed(EIS_KEY_LEFT))
		{
			delta.x -=  cosRot;
			delta.y -=  sinRot;
		}
		if (Input::IsKeyPressed(EIS_KEY_D) || Input::IsKeyPressed(EIS_KEY_RIGHT))
		{
			delta.x +=  cosRot;
			delta.y +=  sinRot;
		}

		if (glm::length(delta) > 1.0f) // TODO: expensive call
			delta /= sqrt(2.0f);

		if (m_ZoomSpeedEffect)
			delta *= m_ZoomLevel; // HACK: find better way to influence speed according to zoom

		m_Camera.AddPosition(delta * (ts * m_CameraSpeed));


		if (m_RotationLock) return;

		if (Input::IsKeyPressed(EIS_KEY_Q))
			m_Camera.AddRotation(m_CameraRotationSpeed * ts);
		if (Input::IsKeyPressed(EIS_KEY_E))
			m_Camera.AddRotation(-m_CameraRotationSpeed * ts);
	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		EIS_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(EIS_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(EIS_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
	}

	void OrthoCameraController::SetZoom(float zoom)
	{
		if (zoom < m_MinZoom || zoom > m_MaxZoom) return;

		m_ZoomLevel = zoom;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	glm::vec2 OrthoCameraController::CalculateMouseWorldPos() const
	{
		EIS_PROFILE_FUNCTION();

		glm::vec2 mousePos = Eis::Input::GetMousePos();

		mousePos /= glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		mousePos = mousePos * 2.0f - glm::vec2(1.0f);
		mousePos.y *= -1.0f;

		const glm::mat4 inverseProjectionMatrix = glm::inverse(m_Camera.GetProjectionMatrix());
		const glm::mat4 inverseViewMatrix = glm::inverse(m_Camera.GetViewMatrix());

		const glm::vec4 worldPos = inverseViewMatrix * inverseProjectionMatrix * glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);

		return glm::vec2(worldPos.x, worldPos.y);
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent e)
	{
		EIS_PROFILE_FUNCTION();

		if (m_ZoomLock) return false;

		m_ZoomLevel *= glm::pow(1.0f + m_ZoomSensitivity, -e.GetYOffset());

		if (m_ZoomLevel < m_MinZoom)
			m_ZoomLevel = m_MinZoom;
		else if (m_ZoomLevel > m_MaxZoom)
			m_ZoomLevel = m_MaxZoom;

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthoCameraController::OnWindowResized(WindowResizeEvent e)
	{
		EIS_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}