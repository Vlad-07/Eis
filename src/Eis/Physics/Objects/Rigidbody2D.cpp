#include "Eispch.h"
#include "Rigidbody2D.h"

#include <glm/gtc/constants.hpp>

#include "Eis/Core/Time.h"


namespace Eis
{
	Rigidbody2D::Rigidbody2D(glm::vec2 pos, float radius, float density, float restitution, bool isStatic)
		: m_Position(pos)
	{
		m_Properties.Density = density;
		m_Properties.Restitution = restitution;
		m_Properties.Area = radius * radius * glm::pi<float>();
		m_Properties.Mass = !isStatic ? m_Properties.Area * density : 0.0f;
		m_Properties.InvMass = !isStatic ? 1.0f / m_Properties.Mass : 0.0f;
		m_Properties.AngularInertia = !isStatic ? 0.5f * m_Properties.Mass * radius * radius : 0.0f;
		m_Properties.InvAngularInertia = !isStatic ? 1.0f / m_Properties.AngularInertia : 0.0f;
		m_Properties.StaticFriction = 0.6f;
		m_Properties.DynamicFriction = 0.4f;
		m_Properties.Static = isStatic;

		m_Collider = Collider2D::Create(radius);
	}

	Rigidbody2D::Rigidbody2D(glm::vec2 pos, float rotation, glm::vec2 size, float density, float restitution, bool isStatic)
		: m_Position(pos), m_Rotation(rotation)
	{
		m_Properties.Density = density;
		m_Properties.Restitution = restitution;
		m_Properties.Area = size.x * size.y;
		m_Properties.Mass = !isStatic ? m_Properties.Area * density : 0.0f;
		m_Properties.InvMass = !isStatic ? 1.0f / m_Properties.Mass : 0.0f;
		m_Properties.AngularInertia = !isStatic ? (1.0f / 12.0f) * m_Properties.Mass * (size.x * size.x + size.y * size.y) : 0.0f;
		m_Properties.InvAngularInertia = !isStatic ? 1.0f / m_Properties.AngularInertia : 0.0f;
		m_Properties.StaticFriction = 0.6f;
		m_Properties.DynamicFriction = 0.4f;
		m_Properties.Static = isStatic;

		const glm::vec2 botL = { -size.x / 2.0f, -size.y / 2.0f };
		const glm::vec2 botR = {  size.x / 2.0f, -size.y / 2.0f };
		const glm::vec2 topR = {  size.x / 2.0f,  size.y / 2.0f };
		const glm::vec2 topL = { -size.x / 2.0f,  size.y / 2.0f };

		m_Collider = Collider2D::Create(size);
	}


	void Rigidbody2D::Update(float timeScale, const glm::vec2 gravity)
	{
		// Euler method

		float ts = (float)Time::GetFixedDeltaTime() * timeScale;

		if (m_Properties.Static) return;

		m_LinearVelocity += ((m_Force * m_Properties.InvMass) + m_Acceleration + gravity) * ts + m_LinearVelAdd;
		m_AngularVelocity += m_AngVelAdd;

		m_Position += m_LinearVelocity * ts;
		m_Rotation += m_AngularVelocity * ts;

		m_Force = glm::vec2();
		m_Acceleration = glm::vec2();
		m_LinearVelAdd = glm::vec2();
		m_AngVelAdd = 0.0f;

		m_Collider->Update();
	}
}