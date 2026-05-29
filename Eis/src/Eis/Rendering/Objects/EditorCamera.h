#pragma once

#include "Camera.h"
#include "Eis/Events/MouseEvent.h"


namespace Eis
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float vFov, float aspectRatio, float nearClip, float farClip);
		~EditorCamera() = default;

		void OnUpdate();
		void OnEvent(Event& e);

		float GetDistance() const {}
		void SetDistance(float distance) {}

		void SetViewportSize(float width, float height)
		{ m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDir() const;
		glm::vec3 GetRightDir() const;
		glm::vec3 GetForwardDir() const;
		glm::vec3 GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(glm::vec2 delta);
		void MouseRotate(glm::vec2 delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		glm::vec2 PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;


	private:
		float m_FOV{ 80.0f }, m_AspectRatio{ 16.0f / 9.0f }, m_NearClip{ 0.1f }, m_FarClip{ 1000.0f };

		glm::mat4 m_ViewMatrix{};
		glm::vec3 m_Position{};
		glm::vec3 m_FocalPoint{};

		glm::vec2 m_InitialMousePos{};

		float m_Distance{ 5.0f };
		float m_Pitch{}, m_Yaw{};

		float m_ViewportWidth{}, m_ViewportHeight{};
	};
}