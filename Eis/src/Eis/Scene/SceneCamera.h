#pragma once

#include "Eis/Rendering/Objects/Camera.h"


namespace Eis
{
	class SceneCamera : public Camera
	{
	public:
		enum ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetViewportSize(uint32_t width, uint32_t height);

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }


		void SetPerspective(float verticalFov, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);


		float GetPerspectiveFov() const { return m_PerspectiveFOV; }
		void SetPerspectiveFov(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
		float GetPerspectiveNear() const { return m_PerspectiveNear; }
		void SetPerspectiveNear(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFar() const { return m_PerspectiveFar; }
		void SetPerspectiveFar(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		float GetOrthoSize() const { return m_OrthoSize; }
		void SetOrthoSize(float size) { m_OrthoSize = size; RecalculateProjection(); }
		float GetOrthoNear() const { return m_OrthoNear; }
		void SetOrthoNear(float nearClip) { m_OrthoNear = nearClip; RecalculateProjection(); }
		float GetOrthoFar() const { return m_OrthoFar; }
		void SetOrthoFar(float farClip) { m_OrthoFar = farClip; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType{ ProjectionType::Orthographic };

		float m_PerspectiveFOV{ glm::radians(80.0f) };
		float m_PerspectiveNear{ 0.1f }, m_PerspectiveFar{ 1000.0f };

		float m_OrthoSize{ 10.0f };
		float m_OrthoNear{ -1.0f }, m_OrthoFar{ 1.0f };

		float m_AspectRatio{ 1.0f };
	};
}