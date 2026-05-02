#pragma once

#include <glm/glm.hpp>


namespace Eis
{
	class OrthographicCamera
	{	
	public:
		OrthographicCamera() = default;
		OrthographicCamera(float aspectRatio, float zoom);
		~OrthographicCamera() = default;


		glm::vec3 GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }


		void SetProjection(float aspectRatio, float zoom);

		void SetPosition(const glm::vec3& pos) { m_Position = pos; RecalculateViewMatrix(); }
		void AddPosition(const glm::vec3& pos) { m_Position += pos; RecalculateViewMatrix(); }

		void SetRotation(float rot) { m_Rotation = rot; RecalculateViewMatrix(); }
		void AddRotation(float rot);

		const OrthographicCamera& operator=(const OrthographicCamera& other);

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix{};
		glm::mat4 m_ViewMatrix{};
		glm::mat4 m_ViewProjectionMatrix{};

		glm::vec3 m_Position{};
		float m_Rotation{}; // Degrees, anti-clockwise
	};
}