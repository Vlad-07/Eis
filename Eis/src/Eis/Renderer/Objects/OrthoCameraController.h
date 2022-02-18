#pragma once

#include "OrthographicCamera.h"
#include "Eis/Core/TimeStep.h"

#include "Eis/Events/ApplicationEvent.h"
#include "Eis/Events/MouseEvent.h"

namespace Eis
{
	class OrthoCameraController
	{
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CamPos = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraSpeed = 2.0f,
			m_CameraRotationSpeed = 90.0f, // degrees / second
			m_ZoomSensitivity = 5.f;

	public:
		OrthoCameraController(float aspectRatio, bool rotation = false);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		const inline OrthographicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolled(MouseScrolledEvent e);
		bool OnWindowResized(WindowResizeEvent e);
	};
}