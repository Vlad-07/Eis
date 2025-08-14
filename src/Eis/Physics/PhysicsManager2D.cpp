#include <Eispch.h>
#include "PhysicsManager2D.h"


namespace Eis
{
	Scope<PhysicsManager2D> PhysicsManager2D::s_Instance = PhysicsManager2D::Create();
	Scope<PhysicsManager2D> PhysicsManager2D::Create()
	{
		return std::make_unique<PhysicsManager2D>();
	}



	void PhysicsManager2D::UpdateInternal(uint8_t iterations, const Eis::TimeStep ts)
	{
		iterations = glm::clamp<uint8_t>(iterations, 1, c_MaxIterations);
		const Eis::TimeStep t(ts / (float)iterations);

		for (uint8_t it = 0; it < iterations; it++)
		{
			// Movement
			for (Rigidbody2D& b : m_Bodies)
				b.Update(t, c_Gravity);

			// Collisions
			CollisionData2D colData;
			for (uint32_t b1 = 0; b1 < m_Bodies.size() - 1; b1++)
			{
				Rigidbody2D& body1 = m_Bodies[b1];
				for (uint32_t b2 = b1 + 1; b2 < m_Bodies.size(); b2++)
				{
					Rigidbody2D& body2 = m_Bodies[b2];

					if (body1.GetStatic() && body2.GetStatic()) continue;

					if (CollisionChecker2D::CheckCollision(body1, body2, colData))
					{
						// HACK: for circle-polygon collisions, circle has to be in b1, otherwise separation breaks
						if (body1.GetType() == RigidbodyType2D::BOX && body2.GetType() == RigidbodyType2D::CIRCLE)
							CollisionSolver2D::SeparateBodies(body2, body1, colData);
						else
							CollisionSolver2D::SeparateBodies(body1, body2, colData);

					//	CollisionSolver2D::SolveCollision(body1, body2, colData);
					}
				}
			}
		}
	}

	void PhysicsManager2D::AddBody(glm::vec2 pos, float radius, float density, float restitution, bool isStatic)
	{ s_Instance->m_Bodies.emplace_back(pos, radius, density, restitution, isStatic); }

	void PhysicsManager2D::AddBody(glm::vec2 pos, float rotation, glm::vec2 size, float density, float restitution, bool isStatic)
	{ s_Instance->m_Bodies.emplace_back(pos, rotation, size, density, restitution, isStatic); }
}