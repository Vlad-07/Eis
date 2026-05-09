#include "Eispch.h"
#include "OrthoCameraController.h"

#include "Eis/Core/Application.h"
#include "Eis/Core/Time.h"
#include "Eis/Input/Input.h"


namespace Eis
{
	OrthoCameraController::OrthoCameraController(float aspectRatio)
	{
		// TODO: fixed aspect ratio
		// might be best handled by the window system and keep this free aspect
		if (aspectRatio != 0) EIS_CORE_WARN("Fixed aspect ratio not implemented!");

		m_AspectRatio = static_cast<float>(Application::GetWindow().GetWidth()) / Application::GetWindow().GetHeight();
		m_Camera = OrthographicCamera(m_AspectRatio, m_Zoom);
	}

	void OrthoCameraController::Update()
	{
		EIS_PROFILE_FUNCTION();

		if (m_PoseLock) return; // you kinda never use rotation, even less without position

		glm::vec3 delta(0.0f);
		const float sinRot = sin(glm::radians(m_Camera.GetRotation())),
					cosRot = cos(glm::radians(m_Camera.GetRotation()));
		if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::Up))
		{
			delta.x += -sinRot;
			delta.y +=  cosRot;
		}
		if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::Down))
		{
			delta.x -= -sinRot;
			delta.y -=  cosRot;
		}
		if (Input::IsKeyPressed(Key::A) || Input::IsKeyPressed(Key::Left))
		{
			delta.x -=  cosRot;
			delta.y -=  sinRot;
		}
		if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::Right))
		{
			delta.x +=  cosRot;
			delta.y +=  sinRot;
		}

		if (glm::length(delta) > 1.0f) // TODO: expensive call
			delta /= sqrt(2.0f);

		if (m_ZoomSpeedEffect)
			delta *= m_Zoom; // HACK: find better way to influence speed according to zoom

		m_Camera.AddPosition(delta * ((float)Time::GetDeltaTime() * m_CameraSpeed));


		if (m_RotationLock) return;

		if (Input::IsKeyPressed(Key::Q))
			m_Camera.AddRotation(m_CameraRotationSpeed * (float)Time::GetDeltaTime());
		if (Input::IsKeyPressed(Key::E))
			m_Camera.AddRotation(-m_CameraRotationSpeed * (float)Time::GetDeltaTime());
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

		m_Zoom = zoom;
		m_Camera.SetProjection(m_AspectRatio, m_Zoom);
	}

	glm::vec2 OrthoCameraController::CalculateMouseWorldPos() const
	{
		EIS_PROFILE_FUNCTION();

		glm::vec2 mousePos = Eis::Input::GetMousePos();

		mousePos /= glm::vec2(Application::GetWindow().GetWidth(), Application::GetWindow().GetHeight());
		mousePos = mousePos * 2.0f - glm::vec2(1.0f);
		mousePos.y *= -1.0f;

		const glm::mat4 inverseProjectionMatrix = glm::inverse(m_Camera.GetProjectionMatrix());
		const glm::mat4 inverseViewMatrix = glm::inverse(m_Camera.GetViewMatrix());

		const glm::vec4 worldPos = inverseViewMatrix * inverseProjectionMatrix * glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);

		return glm::vec2(worldPos.x, worldPos.y);
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		EIS_PROFILE_FUNCTION();

		if (m_ZoomLock) return false;

		m_Zoom *= glm::pow(1.0f + m_ZoomSensitivity, -e.GetYOffset());

		m_Zoom = glm::clamp(m_Zoom, m_MinZoom, m_MaxZoom);

		m_Camera.SetProjection(m_AspectRatio, m_Zoom);
		return false;
	}
	bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		EIS_PROFILE_FUNCTION();

		m_AspectRatio = static_cast<float>(e.GetSize().x) / e.GetSize().y;
		m_Camera.SetProjection(m_AspectRatio, m_Zoom);
		return false;
	}
}