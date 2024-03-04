#pragma once

#include "OrthographicCamera.h"
#include "Eis/Core/TimeStep.h"

#include "Eis/Events/ApplicationEvent.h"
#include "Eis/Events/MouseEvent.h"

namespace Eis
{
	class OrthoCameraController
	{
	public:
		OrthoCameraController(float aspectRatio);
		~OrthoCameraController() = default;

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetPosition(const glm::vec3& pos) { m_CameraPosition = pos; m_Camera.SetPosition(pos); }

		float GetCameraSpeed() const { return m_CameraSpeed; }
		void SetCameraSpeed(float speed) { m_CameraSpeed = speed; }

		void SetMinZoom(float minZoom) { m_MinZoom = minZoom; }
		void SetMaxZoom(float maxZoom) { m_MaxZoom = maxZoom; }

		void SetPoseLock(bool lock) { m_PoseLock = lock; }
		void SetZoomLock(bool lock) { m_ZoomLock = lock; }
		void SetRotationLock(bool lock) { m_RotationLock = lock; }
		void SetZoomSpeedEffect(bool use) { m_ZoomSpeedEffect = use; }

		glm::vec2 CalculateMouseWorldPos();

	private:
		bool OnMouseScrolled(MouseScrolledEvent e);
		bool OnWindowResized(WindowResizeEvent e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 2.0f, m_MinZoom = 0.5f, m_MaxZoom = 10.0f;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraRotation = 0.0f; // Degrees, anti-clockwise direction
		float m_CameraSpeed = 1.0f,
			  m_CameraRotationSpeed = 90.0f,
		      m_ZoomSensitivity = 5.0f;

		bool m_PoseLock;
		bool m_RotationLock;
		bool m_ZoomLock;
		bool m_ZoomSpeedEffect;
	};
}