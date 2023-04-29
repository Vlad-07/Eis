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
		float m_ZoomLevel = 2.0f;
		
		OrthographicCamera m_Camera;

		bool m_lockRotation;
		bool m_lockMovement;
		bool m_lockZoom;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //Degrees, anti-clockwise direction

		float m_MovementSpeed = 2.0f,
			  m_RotationSpeed = 90.0f,
			  m_ZoomSensitivity = 5.0f;
		
	public:
		OrthoCameraController(float aspectRatio, bool movement=false, bool zoom=false, bool rotation=false);
		~OrthoCameraController() = default;

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		const inline OrthographicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		inline glm::vec3 GetPosition() { return m_CameraPosition; }
		inline void SetPosition(glm::vec3 position) { m_CameraPosition = position; }

		inline float GetRotation() { return m_CameraRotation; }
		inline void SetRotation(float rotation) { m_CameraRotation = rotation; }

		inline float GetZoomLevel() { return m_ZoomLevel; }
		inline void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; }


		inline float GetMovementSpeed() { return m_MovementSpeed; }
		inline void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }

		inline float GetRotationSpeed() { return m_RotationSpeed; }
		inline void SetRotationSpeed(float speed) { m_RotationSpeed = speed; }

		inline float GetZoomSensitivity() { return m_ZoomSensitivity; }
		inline void SetZoomSensitivity(float sensitivity) { m_ZoomSensitivity = sensitivity; }


		inline void LockMovement(bool state) { m_lockMovement = state; }

		inline void LockRotation(bool state) { m_lockRotation = state; }

		inline void LockZoom(bool state) { m_lockZoom = state; }


	private:
		bool OnMouseScrolled(MouseScrolledEvent e);
		bool OnWindowResized(WindowResizeEvent e);
	};
}