#include "Eispch.h"
#include "PhysicsManager2D.h"

#include "Eis/Rendering/Renderer/Renderer2D.h"
#include "Eis/Physics/CollisionChecker2D.h"
#include "Eis/Physics/CollisionSolver2D.h"


namespace Eis
{
	Scope<PhysicsManager2D> PhysicsManager2D::s_Instance = CreateScope<PhysicsManager2D>();


	PhysicsManager2D::PhysicsManager2D()
	{
		// This works bc constructor is called before the assignment
		EIS_CORE_ASSERT(!s_Instance, "PhysicsManager2D instance already exists!");
	}

	void PhysicsManager2D::UpdateInternal(uint8_t iterations)
	{
		EIS_PROFILE_FUNCTION();

		iterations = glm::clamp<uint8_t>(iterations, 1, c_MaxIterations);
		const float timeScale = 1.0f / (float)iterations;

		for (uint8_t it = 0; it < iterations; it++)
		{
			m_Contacts.clear();

			UpdateBodies(timeScale);
			BroadPhase();
			NarrowPhase();
		}
	}

	void PhysicsManager2D::UpdateBodies(float timeScale)
	{
		for (Rigidbody2D& b : m_Bodies)
			b.Update(timeScale, c_Gravity);
	}

	// Possible collision detection
	void PhysicsManager2D::BroadPhase()
	{
		if (m_Bodies.size() < 2) // hackish way to not crash at 0 objects
			return;

		CollisionData2D colData;
		for (uint32_t b1 = 0; b1 < m_Bodies.size() - 1; b1++)
		{
			for (uint32_t b2 = b1 + 1; b2 < m_Bodies.size(); b2++)
			{
				if (m_Bodies[b1].GetProperties().Static && m_Bodies[b2].GetProperties().Static) continue;
				if (!CollisionChecker2D::CheckBBIntersection(m_Bodies[b1], m_Bodies[b2])) continue;

				m_Contacts.emplace_back(b1, b2);
			}
		}
	}

	// Collision detection and resolution
	void PhysicsManager2D::NarrowPhase()
	{
		for (auto& [b1, b2] : m_Contacts)
		{
			CollisionManifold2D manifold(&m_Bodies[b1], &m_Bodies[b2]);
			if (CollisionChecker2D::CheckCollision(manifold))
			{
				CollisionSolver2D::SeparateBodies(manifold);
				CollisionChecker2D::FindContactPoints(manifold);
				CollisionSolver2D::SolveCollisionRotationFriction(manifold);
			}
		}
	}


	void PhysicsManager2D::AddBody(glm::vec2 pos, float radius, float density, float restitution, bool isStatic)
	{ s_Instance->m_Bodies.emplace_back(pos, radius, density, restitution, isStatic); }

	void PhysicsManager2D::AddBody(glm::vec2 pos, float rotation, glm::vec2 size, float density, float restitution, bool isStatic)
	{ s_Instance->m_Bodies.emplace_back(pos, rotation, size, density, restitution, isStatic); }


	Rigidbody2D& PhysicsManager2D::GetBody(uint32_t id)
	{
		if (id >= s_Instance->m_Bodies.size())
		{
			EIS_CORE_ERROR("Invalid body id requested: {}!", id);
			static Rigidbody2D b({}, 0, 0, 0);
			return b;
		}

		return s_Instance->m_Bodies[id];
	}
}