#pragma once

#include <glm/glm.hpp>
#include <Eis/Rendering/Objects/Texture.h>
#include <Eis/Core/TimeStep.h>


namespace Eis
{
	enum class RigidbodyType2D : uint8_t
	{
		NONE = 0, CIRCLE, BOX
	};

	typedef glm::mat4x2 ColliderVertices2D;

	struct RigidbodyProperties2D
	{
		float Mass;
		float InvMass;
		float Density;
		float Area;
		float Restitution;

		float Radius;
		glm::vec2 Size;

		ColliderVertices2D Vertices;

		bool Static;
		RigidbodyType2D Type;
	};


	class Rigidbody2D
	{
	public:
		Rigidbody2D() = delete;
		// Circle constructor
		Rigidbody2D(glm::vec2 pos, float radius, float density, float restitution, bool isStatic = false);
		// Box constructor
		Rigidbody2D(glm::vec2 pos, float rotation, glm::vec2 size, float density, float restitution, bool isStatic = false);
		virtual ~Rigidbody2D() = default;


		void Update(const Eis::TimeStep& ts, const glm::vec2 gravity);


		// Force in N
		void AddForce(const glm::vec2 force) { m_Force += force; }
		// Acceleration in m/s^2
		void AddAcceleration(const glm::vec2 accel) { m_Acceleration += accel; }

		void Move(glm::vec2 pos) { m_Position += pos; }
		void MoveTo(glm::vec2 pos) { m_Position = pos; }

		void Rotate(float angle) { m_Rotation += angle; }


		glm::vec2 GetPosition() const { return m_Position; }
		glm::vec2 GetLinearVelocity() const { return m_LinearVelocity; }

		const RigidbodyProperties2D& GetProperties() const { return m_Properties; }
		RigidbodyType2D GetType() const { return m_Properties.Type; }
		glm::vec2 GetSize() const { return m_Properties.Size; }
		float GetRadius() const { return m_Properties.Radius; }
		bool GetStatic() const { return m_Properties.Static; }
		float GetMass() const { return m_Properties.Mass; }
		const ColliderVertices2D& GetTransformedVertices() const;


		void Draw(const glm::vec4& color) const;
		void Draw(Ref<Texture2D> tex) const;

		void DrawBoundingBox() const {}

	private:
		void CalculateTransformedVertices() const;

	public:
		glm::vec2 m_Position;
		float m_Rotation = 0.0f; // Deg

		glm::vec2 m_LinearVelocity = glm::vec2(0.0f); // m/s
//		float m_AngularVelocity = 0.0f; // deg/s

		glm::vec2 m_Acceleration = glm::vec2(); // m/s^2
		glm::vec2 m_Force = glm::vec2(); // N

		mutable ColliderVertices2D m_TransformedVertices = ColliderVertices2D();

		RigidbodyProperties2D m_Properties;
	};
}