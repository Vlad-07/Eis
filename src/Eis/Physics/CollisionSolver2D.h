#pragma once

#include "Objects/Rigidbody2D.h"
#include "CollisionChecker2D.h"


namespace Eis
{
	class CollisionSolver2D
	{
	public:
		static void SeparateBodies(const CollisionManifold2D& manifold)
		{
			// HACK: for circle-polygon collisions, circle has to be in b1, otherwise separation breaks
			const bool x = manifold.Body1.GetType() == ColliderType2D::POLYGON
						&& manifold.Body2.GetType() == ColliderType2D::CIRCLE;
			Rigidbody2D& b1 = x ? manifold.Body2 : manifold.Body1;
			Rigidbody2D& b2 = x ? manifold.Body1 : manifold.Body2;
			const CollisionData2D& data = manifold.Data;

			// static - static collisions aren't checked
			if (b1.GetStatic())
				b2.Move(data.Normal * data.Depth);
			else if (b2.GetStatic())
				b1.Move(data.Normal * -data.Depth);
			else
			{
				const float massSum = b1.GetMass() + b2.GetMass();
				b1.Move(data.Normal * (data.Depth * (-b1.GetMass() / massSum)));
				b2.Move(data.Normal * (data.Depth * ( b2.GetMass() / massSum)));
			}
		}


		static void SolveCollisionBasic(const CollisionManifold2D& manifold)
		{
			// HACK: for circle-polygon collisions, circle has to be in b1, otherwise separation breaks
			const bool x = manifold.Body1.GetType() == ColliderType2D::POLYGON
						&& manifold.Body2.GetType() == ColliderType2D::CIRCLE;
			Rigidbody2D& b1 = x ? manifold.Body2 : manifold.Body1;
			Rigidbody2D& b2 = x ? manifold.Body1 : manifold.Body2;
			const CollisionData2D& data = manifold.Data;

			const glm::vec2 relativeVel = b2.GetLinearVelocity() - b1.GetLinearVelocity();

			if (glm::dot(relativeVel, data.Normal) > 0.0f)
				return;

			const float e = glm::min(b1.GetProperties().Restitution, b2.GetProperties().Restitution);

			const float j = -(1 + e) * glm::dot(relativeVel, data.Normal)
				/ (b1.GetProperties().InvMass + b2.GetProperties().InvMass);

			const glm::vec2 impulse = j * data.Normal;

			b1.AddLinearVel(-impulse * b1.GetProperties().InvMass);
			b2.AddLinearVel( impulse * b2.GetProperties().InvMass);

			EIS_INFO("{0}, {1}", (impulse * b2.GetInvMass()).x, (impulse * b2.GetInvMass()).y);
		}

		/*
		static void SolveCollisionRotation(const CollisionManifold2D& manifold)
		{
			// HACK: for circle-polygon collisions, circle has to be in b1, otherwise separation breaks
			const bool x = manifold.Body1.GetType() == ColliderType2D::POLYGON
						&& manifold.Body2.GetType() == ColliderType2D::CIRCLE;
			Rigidbody2D& b1 = x ? manifold.Body2 : manifold.Body1;
			Rigidbody2D& b2 = x ? manifold.Body1 : manifold.Body2;
			const CollisionData2D& data = manifold.Data;
			const glm::vec2 contacts[2]{ manifold.Contact1, manifold.Contact2 };
			glm::vec2 impulses[2]{};
			glm::vec2 r1List[2]{}, r2List[2]{};
			

			const float e = glm::min(b1.GetProperties().Restitution, b2.GetProperties().Restitution);
			for (uint8_t i = 0; i < manifold.ContactCount; i++)
			{
				r1List[i] = contacts[i] - b1.GetPosition();
				r2List[i] = contacts[i] - b2.GetPosition();

				const glm::vec2 r1 = { -r1List[i].y, r1List[i].x };
				const glm::vec2 r2 = { -r2List[i].y, r2List[i].x };

				const glm::vec2 localAngVel1 = r1 * b1.GetAngularVelocity();
				const glm::vec2 localAngVel2 = r2 * b2.GetAngularVelocity();

				const glm::vec2 relativeVel =
					(b2.GetLinearVelocity() + localAngVel2) -
					(b1.GetLinearVelocity() + localAngVel1);

				const float contactVelMag = glm::dot(relativeVel, data.Normal);

				if (contactVelMag > 0.0f)
					continue;

				const float r1PerpDotN = glm::dot(r1, data.Normal);
				const float r2PerpDotN = glm::dot(r2, data.Normal);

				float j = (-(1.0f + e) * contactVelMag) /
							((b1.GetProperties().InvMass + b2.GetProperties().InvMass) +
							(r1PerpDotN * r1PerpDotN) * b1.GetInvAngInertia() +
							(r2PerpDotN * r2PerpDotN) * b2.GetInvAngInertia());
				j /= manifold.ContactCount;

				impulses[i] = j * data.Normal;
			}

			for (uint8_t i = 0; i < manifold.ContactCount; i++)
			{
				const glm::vec2& impulse = impulses[i];

				// 2D cross: x.x * y.y - x.y * y.x
				b1.AddLinearVel(-impulse * b1.GetInvMass());
				b1.AddAngularVel(-(r1List[i].x * impulse.y - r1List[i].y * impulse.x) * b1.GetInvAngInertia());
				b2.AddLinearVel(impulse * b2.GetInvMass());
				b2.AddAngularVel((r2List[i].x * impulse.y - r2List[i].y * impulse.x) * b2.GetInvAngInertia());
			}
		}//*/


		static void SolveCollisionRotationFriction(const CollisionManifold2D& manifold)
		{
			// HACK: for circle-polygon collisions, circle has to be in b1, otherwise separation breaks
			const bool x = manifold.Body1.GetType() == ColliderType2D::POLYGON
				&& manifold.Body2.GetType() == ColliderType2D::CIRCLE;
			Rigidbody2D& b1 = x ? manifold.Body2 : manifold.Body1;
			Rigidbody2D& b2 = x ? manifold.Body1 : manifold.Body2;
			const CollisionData2D& data = manifold.Data;
			const glm::vec2 contacts[2]{ manifold.Contact1, manifold.Contact2 };
			glm::vec2 impulses[2]{};
			glm::vec2 r1List[2]{}, r2List[2]{};
			float jList[2]{};

			const float e = glm::min(b1.GetProperties().Restitution, b2.GetProperties().Restitution);
			const float sf = (b1.GetProperties().StaticFriction + b2.GetProperties().StaticFriction) * 0.5f;
			const float df = (b1.GetProperties().DynamicFriction + b2.GetProperties().DynamicFriction) * 0.5f;

			// Rotation
			for (uint8_t i = 0; i < manifold.ContactCount; i++)
			{
				r1List[i] = contacts[i] - b1.GetPosition();
				r2List[i] = contacts[i] - b2.GetPosition();

				const glm::vec2 r1 = { -r1List[i].y, r1List[i].x };
				const glm::vec2 r2 = { -r2List[i].y, r2List[i].x };

				const glm::vec2 localAngVel1 = r1 * b1.GetAngularVelocity();
				const glm::vec2 localAngVel2 = r2 * b2.GetAngularVelocity();

				const glm::vec2 relativeVel =
					(b2.GetLinearVelocity() + localAngVel2) -
					(b1.GetLinearVelocity() + localAngVel1);

				const float contactVelMag = glm::dot(relativeVel, data.Normal);

				if (contactVelMag > 0.0f)
					continue;

				const float r1PerpDotN = glm::dot(r1, data.Normal);
				const float r2PerpDotN = glm::dot(r2, data.Normal);

				float j = (-(1.0f + e) * contactVelMag) /
					((b1.GetProperties().InvMass + b2.GetProperties().InvMass) +
						(r1PerpDotN * r1PerpDotN) * b1.GetInvAngInertia() +
						(r2PerpDotN * r2PerpDotN) * b2.GetInvAngInertia());
				j /= manifold.ContactCount;

				jList[i] = j;

				impulses[i] = j * data.Normal;
			}

			for (uint8_t i = 0; i < manifold.ContactCount; i++)
			{
				const glm::vec2& impulse = impulses[i];

				// 2D cross: x.x * y.y - x.y * y.x
				b1.AddLinearVel(-impulse * b1.GetInvMass());
				b1.AddAngularVel(-(r1List[i].x * impulse.y - r1List[i].y * impulse.x) * b1.GetInvAngInertia());
				b2.AddLinearVel(impulse * b2.GetInvMass());
				b2.AddAngularVel((r2List[i].x * impulse.y - r2List[i].y * impulse.x) * b2.GetInvAngInertia());
			}

			// Friction
			for (uint8_t i = 0; i < manifold.ContactCount; i++)
			{
				r1List[i] = contacts[i] - b1.GetPosition();
				r2List[i] = contacts[i] - b2.GetPosition();

				const glm::vec2 r1 = { -r1List[i].y, r1List[i].x };
				const glm::vec2 r2 = { -r2List[i].y, r2List[i].x };

				const glm::vec2 localAngVel1 = r1 * b1.GetAngularVelocity();
				const glm::vec2 localAngVel2 = r2 * b2.GetAngularVelocity();

				const glm::vec2 relativeVel =
					(b2.GetLinearVelocity() + localAngVel2) -
					(b1.GetLinearVelocity() + localAngVel1);

				glm::vec2 tangent = relativeVel - data.Normal * glm::dot(relativeVel, data.Normal);

				if (glm::all(glm::epsilonEqual(tangent, glm::vec2(0.0f), c_HalfMilimeter)))
					continue;

				tangent = glm::normalize(tangent);

				const float r1PerpDotT = glm::dot(r1, tangent);
				const float r2PerpDotT = glm::dot(r2, tangent);

				float jt = (-glm::dot(relativeVel, tangent)) /
					((b1.GetProperties().InvMass + b2.GetProperties().InvMass) +
						(r1PerpDotT * r1PerpDotT) * b1.GetInvAngInertia() +
						(r2PerpDotT * r2PerpDotT) * b2.GetInvAngInertia());
				jt /= manifold.ContactCount;

				glm::vec2 frictionImpulse;
				if (glm::abs(jt) <= jList[i] * sf)
					frictionImpulse = jt * tangent;
				else
					frictionImpulse = -jList[i] * tangent * df;

				impulses[i] = frictionImpulse;
			}

			for (uint8_t i = 0; i < manifold.ContactCount; i++)
			{
				const glm::vec2& impulse = impulses[i];

				// 2D cross: x.x * y.y - x.y * y.x
				b1.AddLinearVel(-impulse * b1.GetInvMass());
				b1.AddAngularVel(-(r1List[i].x * impulse.y - r1List[i].y * impulse.x) * b1.GetInvAngInertia());
				b2.AddLinearVel(impulse * b2.GetInvMass());
				b2.AddAngularVel((r2List[i].x * impulse.y - r2List[i].y * impulse.x) * b2.GetInvAngInertia());
			}
		}
	};
}