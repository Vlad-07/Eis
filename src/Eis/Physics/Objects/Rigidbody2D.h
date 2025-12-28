#pragma once

#include <glm/glm.hpp>

#include "Eis/Physics/Objects/Colliders/Collider2D.h"


// TODO: better manager interface
// TODO: ecs (transform, renderers, etc.)

namespace Eis
{
	class Rigidbody2D
	{
	public:
		struct Properties
		{
			bool Static;

			float Mass;
			float InvMass;
			float Density;
			float Area;
			float Restitution;
			float AngularInertia;
			float InvAngularInertia;
			float StaticFriction;
			float DynamicFriction;
		};

	public:
		Rigidbody2D() = delete;
		// Circle constructor
		Rigidbody2D(glm::vec2 pos, float radius, float density, float restitution, bool isStatic = false);
		// Box constructor
		Rigidbody2D(glm::vec2 pos, float rotation, glm::vec2 size, float density, float restitution, bool isStatic = false);

		Rigidbody2D(Rigidbody2D&) = delete;
		Rigidbody2D& operator=(Rigidbody2D&) = delete;

		Rigidbody2D(Rigidbody2D&&) = default;
		Rigidbody2D& operator=(Rigidbody2D&&) = default;

		virtual ~Rigidbody2D() = default;

//		Rigidbody2D& operator=(const Rigidbody2D& other);


		void Update(float timeScale, const glm::vec2 gravity);


		void Move(glm::vec2 pos) { m_Position += pos; m_Collider->Update(); }
		void MoveTo(glm::vec2 pos) { m_Position = pos; m_Collider->Update(); }

		void Rotate(float deg) { m_Rotation += glm::radians(deg); m_Collider->Update(); }
		void RotateTo(float deg) { m_Rotation = glm::radians(deg); m_Collider->Update(); }

		void RotateRad(float rad) { m_Rotation += rad; m_Collider->Update(); }
		void RotateToRad(float rad) { m_Rotation = rad; m_Collider->Update(); }


		// Force in N
		void AddForce(const glm::vec2 force) { m_Force += force; }
		// Acceleration in m/s^2
		void AddAcceleration(const glm::vec2 accel) { m_Acceleration += accel; }
		// Linear velocity in m/s
		void AddLinearVel(const glm::vec2 linearVel) { m_LinearVelAdd += linearVel; }
		// Angular velocity in rad/s
		void AddAngularVel(float angVel) { m_AngVelAdd += angVel; }

		glm::vec2 GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }
		// Linear velocity in m/s
		glm::vec2 GetLinearVelocity() const { return m_LinearVelocity; }
		// Angular velocity in rad/s
		float GetAngularVelocity() const { return m_AngularVelocity; }

		const Properties& GetProperties() const { return m_Properties; }
		const Collider2D& GetCollider() const { return *m_Collider; }
		const BBox2D& GetBBox() const { return m_Collider->GetBBox(m_Position, m_Rotation); } // Will be expanded for composite colliders

	private:
		Properties m_Properties;

		glm::vec2 m_Position = glm::vec2(); // m
		float m_Rotation = 0.0f; // rad

		glm::vec2 m_LinearVelocity = glm::vec2(0.0f); // m/s
		float m_AngularVelocity = 0.0f; // rad/s

		glm::vec2 m_Force = glm::vec2(); // N (kg * m / s^2)
		glm::vec2 m_Acceleration = glm::vec2(); // m/s^2
		glm::vec2 m_LinearVelAdd = glm::vec2(); // m/s

		float m_AngVelAdd = 0.0f;


		Scope<Collider2D> m_Collider;
	};
}