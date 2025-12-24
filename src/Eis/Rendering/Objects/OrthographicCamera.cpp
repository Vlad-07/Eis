#include "Eispch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Eis
{
	OrthographicCamera::OrthographicCamera(float aspectRatio, float zoom)
	{
		EIS_PROFILE_FUNCTION();

		SetProjection(aspectRatio, zoom);
	}

	void OrthographicCamera::SetProjection(float aspectRatio, float zoom)
	{
		EIS_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom, -1.0f, 1.0f);
		RecalculateViewMatrix();
	}

	void OrthographicCamera::AddRotation(float rot)
	{
		m_Rotation += rot;
		if (m_Rotation > 360.0f) m_Rotation -= 360.0f;
		else if (m_Rotation < -360.0f) m_Rotation += 360.0f;
		RecalculateViewMatrix();
	}

	const OrthographicCamera& OrthographicCamera::operator=(const OrthographicCamera& other)
	{
		m_ProjectionMatrix = other.m_ProjectionMatrix;
		m_ViewMatrix = other.m_ViewMatrix;
		m_ViewProjectionMatrix = other.m_ViewProjectionMatrix;
		m_Position = other.m_Position;
		m_Rotation = other.m_Rotation;

		return *this;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		EIS_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}