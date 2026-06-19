#pragma once

#include "Camera.h"
#include "Eis/Events/MouseEvent.h"


namespace Eis
{
	class EditorCamera2 : public Camera
	{
	public:
		EditorCamera2() { UpdateProjection(); }
		~EditorCamera2() = default;

		void OnUpdate();
		void OnEvent(Event& event);

		void SetViewportSize(float width, float height)
		{ m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }


		glm::mat4 GetView() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		// ImGui widget utilities

		float* GetNearClip() { return &m_NearClip; }
		float* GetFarClip() { return &m_FarClip; }
		float* GetFov() { return &m_Fov; }
		float* GetSpeed() { return &m_Speed; }
		float* GetSensitivity() { return &m_Sensitivity; }


	private:
		void UpdateProjection();
		void UpdateView();

		glm::vec3 GetUpDir();
		glm::vec3 GetForwardDir();
		glm::vec3 GetRightDir();

	private:
		glm::vec3 m_Position{ 0, 0, 5 };
		glm::vec3 m_Rotation{};

		float m_Speed{ 5.0f };
		float m_Sensitivity{ 0.8f };

		float m_Fov{ 80 }; // Vertical fov
		float m_NearClip{ 0.1f }, m_FarClip{ 1000 };
		float m_ViewportWidth{ 16 }, m_ViewportHeight{ 9 };
		float m_AspectRatio{};

		glm::mat4 m_ViewMatrix{};

		glm::vec2 m_LastMousePos{};
	};
}