#pragma once

#include "Objects/Rigidbody2D.h"
#include "CollisionChecker2D.h"

#include <Eis/Rendering/Renderer/Renderer2D.h>


namespace Eis
{
	class CollisionSolver2D
	{
	public:
		static void SeparateBodies(Rigidbody2D& b1, Rigidbody2D& b2, const CollisionData2D& data)
		{
			if (b1.GetStatic())
			{
			//	Eis::Renderer2D::DrawCircle(b2.GetPosition(), glm::vec2(0.1f), glm::vec4(1, 0, 0, 1));
				b2.Move(data.Normal * data.Depth);
			//	Eis::Renderer2D::DrawCircle(b2.GetPosition(), glm::vec2(0.1f), glm::vec4(0, 1, 0, 1));
			}
			else if (b2.GetStatic())
			{
			//	Eis::Renderer2D::DrawCircle(b1.GetPosition(), glm::vec2(0.1f), glm::vec4(1, 0, 0, 1));
				b1.Move(data.Normal * -data.Depth);
			//	Eis::Renderer2D::DrawCircle(b1.GetPosition(), glm::vec2(0.1f), glm::vec4(0, 1, 0, 1));
			}
			else
			{
				const float massSum = b1.GetMass() + b2.GetMass();

				//Eis::Renderer2D::DrawLine(b1.GetPosition(), b1.GetPosition() + data.Normal * data.Depth * data.Sign * (b1.GetMass() /-massSum), glm::vec4(1));
				//Eis::Renderer2D::DrawCircle(b1.GetPosition(), glm::vec2(0.1f), glm::vec4(1, 0, 0, 1));

				//Eis::Renderer2D::DrawLine(b2.GetPosition(), b2.GetPosition() + data.Normal * data.Depth * data.Sign * (b2.GetMass() / massSum), glm::vec4(1));
				//Eis::Renderer2D::DrawCircle(b2.GetPosition(), glm::vec2(0.1f), glm::vec4(1, 0, 0, 1));

				b1.Move(data.Normal * (data.Depth * data.Sign * (-b1.GetMass() / massSum)));
				b2.Move(data.Normal * (data.Depth * data.Sign * (b2.GetMass() / massSum)));
			}
		}

		static void SolveCollision(Rigidbody2D& b1, Rigidbody2D& b2, const CollisionData2D& data)
		{
			const glm::vec2 relativeVel = b2.GetLinearVelocity() - b1.GetLinearVelocity();

			if (glm::dot(relativeVel, data.Normal) > 0.0f)
				return;

			const float e = glm::min(b1.GetProperties().Restitution, b2.GetProperties().Restitution);

			const float j = -(1 + e) * glm::dot(relativeVel, data.Normal)
							/ (b1.GetProperties().InvMass + b2.GetProperties().InvMass);

			const glm::vec2 impulse = j * data.Normal;

		//	EIS_INFO("Impulse: {0}, {1}", impulse.x, impulse.y);

			// ???

			b1.m_LinearVelocity -= impulse * b1.GetProperties().InvMass;
			b2.m_LinearVelocity += impulse * b2.GetProperties().InvMass;
		}
	};
}