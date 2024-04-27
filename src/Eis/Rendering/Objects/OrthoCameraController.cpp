#include "Eispch.h"
#include "OrthoCameraController.h"

#include "Eis/Core/Application.h"
#include "Eis/Input/Input.h"


namespace Eis
{
	OrthoCameraController::OrthoCameraController(float aspectRatio)
		: m_AspectRatio(aspectRatio), m_PoseLock(false), m_ZoomLock(false), m_RotationLock(true), m_ZoomSpeedEffect(true),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel) {}

	void OrthoCameraController::OnUpdate(TimeStep ts)
	{
		EIS_PROFILE_FUNCTION();

		if (m_PoseLock) return; // you kinda never use rotation, even less without position

		glm::fvec2 delta(0.0f);
		float sinRot = sin(glm::radians(m_CameraRotation)), cosRot = cos(glm::radians(m_CameraRotation));
		if (Input::IsKeyPressed(EIS_KEY_UP) || Input::IsKeyPressed(EIS_KEY_W))
		{
			delta.x += -sinRot;
			delta.y +=  cosRot;
		}
		if (Input::IsKeyPressed(EIS_KEY_DOWN) || Input::IsKeyPressed(EIS_KEY_S))
		{
			delta.x -= -sinRot;
			delta.y -=  cosRot;
		}
		if (Input::IsKeyPressed(EIS_KEY_LEFT) || Input::IsKeyPressed(EIS_KEY_A))
		{
			delta.x -=  cosRot;
			delta.y -=  sinRot;
		}
		if (Input::IsKeyPressed(EIS_KEY_RIGHT) || Input::IsKeyPressed(EIS_KEY_D))
		{
			delta.x +=  cosRot;
			delta.y +=  sinRot;
		}

		// limit diagonal movement
		if (glm::length(delta) > 1.0f)
			delta /= 1.4142f; // sqrtf(2)

		if (m_ZoomSpeedEffect)
			delta *= m_ZoomLevel; // HACK: find better way to influence speed according to zoom

		m_CameraPosition += glm::vec3(delta * (ts.GetSeconds() * m_CameraSpeed), 0.0f);
		m_Camera.SetPosition(m_CameraPosition);


		if (m_RotationLock) return;

		if (Input::IsKeyPressed(EIS_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Input::IsKeyPressed(EIS_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		if (m_CameraRotation > 180.0f)
			m_CameraRotation -= 360.0f;
		else if (m_CameraRotation <= -180.0f)
			m_CameraRotation += 360.0f;
		
		m_Camera.SetRotation(m_CameraRotation);
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
		if (zoom >= m_MinZoom && zoom <= m_MaxZoom)
			m_ZoomLevel = zoom;
		else
			EIS_ASSERT(false, "Invalid zoom level!");
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	glm::vec2 OrthoCameraController::CalculateMouseWorldPos()
	{
		EIS_PROFILE_FUNCTION();

		glm::vec2 mousePos = Eis::Input::GetMousePos();

		mousePos /= glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		mousePos = mousePos * 2.0f - glm::vec2(1.0f);
		mousePos.y *= -1.0f;

		glm::mat4 inverseProjectionMatrix = glm::inverse(m_Camera.GetProjectionMatrix());
		glm::mat4 inverseViewMatrix = glm::inverse(m_Camera.GetViewMatrix());

		glm::vec4 worldPos = inverseViewMatrix * inverseProjectionMatrix * glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);

		return glm::vec2(worldPos.x, worldPos.y);
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent e)
	{
		EIS_PROFILE_FUNCTION();

		if (m_ZoomLock) return false;

		m_ZoomLevel -= e.GetYOffset() / m_ZoomSensitivity;

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

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}