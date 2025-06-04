#pragma once

#include <glm/glm.hpp>


namespace Eis
{
	class OrthographicCamera
	{	
	public:
		OrthographicCamera() = default;
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera() = default;

		void SetProjection(float left, float right, float bottom, float top);

		glm::vec3 GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& pos) { m_Position = pos; RecalculateViewMatrix(); }
		void AddPosition(const glm::vec2& pos) { m_Position.x += pos.x; m_Position.y += pos.y; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rot) { m_Rotation = rot; RecalculateViewMatrix(); }
		void AddRotation(float rot)
		{
			m_Rotation += rot;
			if (m_Rotation > 360.0f) m_Rotation -= 360.0f;
			else if (m_Rotation < -360.0f) m_Rotation += 360.0f;
			RecalculateViewMatrix();
		}


		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		const OrthographicCamera& operator=(const OrthographicCamera& other)
		{
			m_ProjectionMatrix = other.m_ProjectionMatrix;
			m_ViewMatrix = other.m_ViewMatrix;
			m_ViewProjectionMatrix = other.m_ViewProjectionMatrix;
			m_Position = other.m_Position;
			m_Rotation = other.m_Rotation;

			return *this;
		}

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix{};
		glm::mat4 m_ViewMatrix{};
		glm::mat4 m_ViewProjectionMatrix{};

		glm::vec3 m_Position = glm::vec3(0.0f);
		float m_Rotation = 0.0f; // Degrees, anti-clockwise
	};
}