#pragma once

#include "OrthographicCamera.h"

#include "Eis/Events/ApplicationEvent.h"
#include "Eis/Events/MouseEvent.h"


namespace Eis
{
	class OrthoCameraController
	{
	public:
		// TODO: mouse panning
		// TODO: fixed aspect ratio
		// Specify a fixed aspect ratio or 0 for free aspect ratio
		OrthoCameraController(float aspectRatio = 0.0f);
		~OrthoCameraController() = default;

		void Update();
		void OnEvent(Event& e);


		glm::vec2 GetPosition() const { return { m_Camera.GetPosition().x, m_Camera.GetPosition().y }; }
		float GetCameraSpeed() const { return m_CameraSpeed; }
		float GetZoom() const { return m_Zoom; }
		float GetRotation() const { return m_Camera.GetRotation(); }
		float GetAspectRatio() const { return m_AspectRatio; }


		void SetPosition(const glm::vec2& pos) { SetPosition(glm::vec3(pos, 0.0f)); }
		void SetPosition(const glm::vec3& pos) { m_Camera.SetPosition(pos); }

		void SetCameraSpeed(float speed) { m_CameraSpeed = speed; }
		void SetZoom(float zoom);
		void SetMinZoom(float minZoom) { m_MinZoom = minZoom; }
		void SetMaxZoom(float maxZoom) { m_MaxZoom = maxZoom; }


		void SetPoseLock(bool lock) { m_PoseLock = lock; }
		void SetZoomLock(bool lock) { m_ZoomLock = lock; }
		void SetRotationLock(bool lock) { m_RotationLock = lock; }
		void SetZoomSpeedEffect(bool enabled) { m_ZoomSpeedEffect = enabled; }
		void SetMousePAnning(bool enabled) { m_MousePanning = enabled; }


		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		// glm::vec2 CalcutalePointWorldPos(glm::vec2 pos) const

		glm::vec2 CalculateMouseWorldPos() const;

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		OrthographicCamera m_Camera;
		float m_AspectRatio{};

		float m_CameraSpeed{ 1.0f },
			  m_CameraRotationSpeed{ 90.0f },
			  m_ZoomSensitivity{ 0.1f };

		float m_Zoom{ 2.0f },
			  m_MinZoom{ 0.5f },
			  m_MaxZoom{ 10.0f };

		bool m_PoseLock{ false };
		bool m_ZoomLock{ false };
		bool m_RotationLock{ true };
		bool m_ZoomSpeedEffect{ true };
		bool m_MousePanning{ true };
	};
}