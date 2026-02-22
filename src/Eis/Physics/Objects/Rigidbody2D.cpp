#include "Eispch.h"
#include "Rigidbody2D.h"

#include <glm/gtc/constants.hpp>

#include "Eis/Core/Time.h"


namespace Eis
{
	Rigidbody2D::Rigidbody2D(glm::vec2 pos, float radius, const Descriptor& descriptor)
		: m_Position{ pos }
	{
		m_Properties.Static = descriptor.Static;
		//m_Properties.Density = descriptor.Density;
		m_Properties.Restitution = descriptor.Restitution;
		m_Properties.StaticFriction = descriptor.StaticFriction;
		m_Properties.DynamicFriction = descriptor.DynamicFriction;

		//m_Properties.Area = radius * radius * glm::pi<float>();
		if (descriptor.Mass)
			m_Properties.Mass = descriptor.Mass;
		else
		{
			const float area = radius * radius * glm::pi<float>();
			m_Properties.Mass = !descriptor.Static ? area * descriptor.Density : 0.0f;
		}
		m_Properties.InvMass = !descriptor.Static ? 1.0f / m_Properties.Mass : 0.0f;
		//m_Properties.AngularInertia = !descriptor.Static ? 0.5f * m_Properties.Mass * radius * radius : 0.0f;
		const float angularInertia = !descriptor.Static ? 0.5f * m_Properties.Mass * radius * radius : 0.0f;
		m_Properties.InvAngularInertia = !descriptor.Static ? 1.0f / angularInertia : 0.0f;

		m_Collider = Collider2D::Create(radius);
	}

	Rigidbody2D::Rigidbody2D(glm::vec2 pos, float rotation, glm::vec2 size, const Descriptor& descriptor)
		: m_Position{ pos }, m_Rotation{ glm::radians(rotation) }
	{
		m_Properties.Static = descriptor.Static;
		//m_Properties.Density = descriptor.Density;
		m_Properties.Restitution = descriptor.Restitution;
		m_Properties.StaticFriction = descriptor.StaticFriction;
		m_Properties.DynamicFriction = descriptor.DynamicFriction;

		//m_Properties.Area = size.x * size.y;
		if (descriptor.Mass)
			m_Properties.Mass = descriptor.Mass;
		else
		{
			const float area = size.x * size.y;
			m_Properties.Mass = !descriptor.Static ? area * descriptor.Density : 0.0f;
		}
		m_Properties.InvMass = !descriptor.Static ? 1.0f / m_Properties.Mass : 0.0f;
		//m_Properties.AngularInertia = !descriptor.Static ? (1.0f / 12.0f) * m_Properties.Mass * (size.x * size.x + size.y * size.y) : 0.0f;
		const float angularInertia = !descriptor.Static ? (1.0f / 12.0f) * m_Properties.Mass * (size.x * size.x + size.y * size.y) : 0.0f;

		m_Properties.InvAngularInertia = !descriptor.Static ? 1.0f / angularInertia : 0.0f;

		m_Collider = Collider2D::Create(size);
	}

	Rigidbody2D::Rigidbody2D(glm::vec2 pos, float rotation, const Vertices& verts, const Descriptor& descriptor)
		: m_Position{ pos }, m_Rotation{ glm::radians(rotation) }
	{
		m_Properties.Static = descriptor.Static;
		//m_Properties.Density = descriptor.Density;
		m_Properties.Restitution = descriptor.Restitution;
		m_Properties.StaticFriction = descriptor.StaticFriction;
		m_Properties.DynamicFriction = descriptor.DynamicFriction;

		// Shoelace formula (triangle)
		// TODO: does the engine work with a line(2 vertices) ?
		EIS_ASSERT(verts.size() >= 3, "Invalid vertex count: {}!", verts.size());
		glm::vec2 com{}; // center of mass used for angular inertia
		float iArea{}; // inertia about origin

		float area{};
		for (uint8_t i{}; i < verts.size(); i++)
		{
			const float di = verts[i].x * verts[(i + 1) % verts.size()].y - verts[(i + 1) % verts.size()].x * verts[i].y;

			area += di;

			com.x += di * (verts[i].x + verts[(i + 1) % verts.size()].x);
			com.y += di * (verts[i].y + verts[(i + 1) % verts.size()].y);

			iArea += di * (verts[i].x * verts[i].x
									+ verts[i].x * verts[(i + 1) % verts.size()].x)
									+ verts[(i + 1) % verts.size()].x * verts[(i + 1) % verts.size()].x
									+ (verts[i].y * verts[i].y
									+ verts[i].y * verts[(i + 1) % verts.size()].y)
									+ verts[(i + 1) % verts.size()].y * verts[(i + 1) % verts.size()].y;
		}
		//m_Properties.Area = sum / 2.0f;
		area /= 2.0f;
		EIS_ASSERT(area >= 0.0f, "Negative area: {}! Vertices must be ccw!", area);

		if (descriptor.Mass)
			m_Properties.Mass = descriptor.Mass;
		else
			m_Properties.Mass = !descriptor.Static ? area * descriptor.Density : 0.0f;
		m_Properties.InvMass = !descriptor.Static ? 1.0f / m_Properties.Mass : 0.0f;
//		m_Properties.AngularInertia;

		com /= 6.0f * area;
		iArea /= 12.0f;

		const float iCoM = iArea - area * (com.x * com.x + com.y * com.y);
		const float angularInertia = iCoM * m_Properties.Mass / area;

		m_Properties.InvAngularInertia = !descriptor.Static ? 1.0f / angularInertia : 0.0f;

		m_Collider = Collider2D::Create(verts);
	}


	void Rigidbody2D::Update(float timeScale, const glm::vec2 gravity)
	{
		// Euler method

		if (m_Properties.Static) return;

		const float ts = (float)Time::GetFixedDeltaTime() * timeScale;

		m_LinearVelocity += ((m_Force * m_Properties.InvMass) + m_Acceleration + gravity) * ts + m_LinearVelAdd;
		m_AngularVelocity += m_AngVelAdd;

		m_Position += m_LinearVelocity * ts;
		m_Rotation += m_AngularVelocity * ts;
		m_Rotation = glm::mod(m_Rotation, glm::tau<float>());

		m_Force = glm::vec2{};
		m_Acceleration = glm::vec2{};
		m_LinearVelAdd = glm::vec2{};
		m_AngVelAdd = 0.0f;

		m_Collider->Update();
	}

	void Rigidbody2D::RotateRad(float rad)
	{
		m_Rotation = glm::mod(m_Rotation + rad, glm::tau<float>());
		m_Collider->Update();
	}

	void Rigidbody2D::RotateToRad(float rad)
	{
		m_Rotation = glm::mod(rad, glm::tau<float>());
		m_Collider->Update();
	}
}