#pragma once

#include <glm/glm.hpp>

#include "Eis/Physics/Objects/Colliders/Collider2D.h"


// TODO: better manager interface
// TODO: ecs (transform, renderers, etc.)
// TODO: maybe a material lib?
namespace Eis
{
	class Rigidbody2D
	{
	public:
		struct Descriptor
		{
			Descriptor() = default;
			Descriptor(bool s, float d, float r, float sf, float df)
				: Static{ s }, Density{ d }, Restitution{ r },
				StaticFriction{ sf }, DynamicFriction{ df } {}
			~Descriptor() = default;

			bool Static{};
			float Density{}; // kg / m^2
			float Restitution{};
			float StaticFriction{};
			float DynamicFriction{};
		};

		struct Properties
		{
			bool Static{};
			//float Density{}; // kg / m^2
			float Restitution{};
			float StaticFriction{};
			float DynamicFriction{};

			float Mass{}; // kg
			float InvMass{}; // kg^-1
			//float Area{}; // m^2
			//float AngularInertia{}; // kg * m^2
			float InvAngularInertia{};
		};

	public:
		Rigidbody2D() = delete;
		// Circle constructor
		Rigidbody2D(glm::vec2 pos, float radius, const Descriptor& descriptor);
		// Box constructor
		Rigidbody2D(glm::vec2 pos, glm::vec2 size, float rotation, const Descriptor& descriptor);
		// Polygon constructor
		Rigidbody2D(glm::vec2 pos, const Vertices& verts, float rotation, const Descriptor& descriptor);

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

		glm::vec2 m_Position{}; // m
		float m_Rotation{}; // rad

		glm::vec2 m_LinearVelocity{}; // m/s
		float m_AngularVelocity{}; // rad/s

		glm::vec2 m_Force{}; // N (kg * m / s^2)
		glm::vec2 m_Acceleration{}; // m/s^2
		glm::vec2 m_LinearVelAdd{}; // m/s

		float m_AngVelAdd{}; // rad/s


		Scope<Collider2D> m_Collider;
	};
}