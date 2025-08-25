#pragma once

#include "Objects/Rigidbody2D.h"
#include <glm/gtc/epsilon.hpp>


namespace Eis
{
	static constexpr float c_HalfMilimeter = std::numeric_limits<float>::epsilon();

	struct CollisionData2D
	{
		glm::vec2 Normal = glm::vec2();
		float Depth = std::numeric_limits<float>::max();
	//	float Sign = 1.0f;
	};

	struct CollisionManifold2D
	{
		CollisionManifold2D(Rigidbody2D& b1, Rigidbody2D& b2)
			: Body1(b1), Body2(b2) {}

		Rigidbody2D& Body1;
		Rigidbody2D& Body2;
		CollisionData2D Data{}; 
		glm::vec2 Contact1{}, Contact2{};
		uint8_t ContactCount = 0;
	};


	class CollisionChecker2D
	{
	public:
		static bool CheckBBIntersection(const Rigidbody2D& collider1, const Rigidbody2D& collider2)
		{
			const AlignedBoundingBox2D& bb1 = collider1.GetBoundingBox();
			const AlignedBoundingBox2D& bb2 = collider2.GetBoundingBox();

			if (bb1.TopRight.x <= bb2.BottomLeft.x || bb2.TopRight.x <= bb1.BottomLeft.x
			 || bb1.TopRight.y <= bb2.BottomLeft.y || bb2.TopRight.y <= bb1.BottomLeft.y)
				return false;

			return true;
		}

		static bool CheckCollision(CollisionManifold2D& manifold)
		{
			const Rigidbody2D& body1 = manifold.Body1;
			const Rigidbody2D& body2 = manifold.Body2;

			if (body1.GetType() == ColliderType2D::CIRCLE)
			{
				if (body2.GetType() == ColliderType2D::CIRCLE)
					return CollideCircles(body1, body2, manifold.Data);

				else if (body2.GetType() == ColliderType2D::POLYGON)
					return CollideCirclePolygon(body1, body2, manifold.Data);
			}
			else if (body1.GetType() == ColliderType2D::POLYGON)
			{
				if (body2.GetType() == ColliderType2D::POLYGON)
					return CollidePolygons(body1, body2, manifold.Data);

				else if (body2.GetType() == ColliderType2D::CIRCLE)
					return CollideCirclePolygon(body2, body1, manifold.Data);
			}

			EIS_CORE_ASSERT(false, "Collision type could not be determined!");
			return false;
		}

		static void FindContactPoints(CollisionManifold2D& manifold)
		{
			const Rigidbody2D& body1 = manifold.Body1;
			const Rigidbody2D& body2 = manifold.Body2;

			if (body1.GetType() == ColliderType2D::CIRCLE)
			{
				if (body2.GetType() == ColliderType2D::CIRCLE)
					FindCPCircles(body1, body2, manifold);

				else if (body2.GetType() == ColliderType2D::POLYGON)
					FindCPCirclePolygon(body1, body2, manifold);
			}
			else if (body1.GetType() == ColliderType2D::POLYGON)
			{
				if (body2.GetType() == ColliderType2D::POLYGON)
					FindCPPolygons(body1, body2, manifold);

				else if (body2.GetType() == ColliderType2D::CIRCLE)
					FindCPCirclePolygon(body2, body1, manifold);
			}
		}


	private:
		static bool CollideCircles(const Rigidbody2D& collider1, const Rigidbody2D& collider2, CollisionData2D& data)
		{
			const float dist = glm::distance(collider1.GetPosition(), collider2.GetPosition());
			const float minDist = collider1.GetRadius() + collider2.GetRadius();

			if (dist >= minDist)
				return false;

			data.Normal = glm::normalize(collider2.GetPosition() - collider1.GetPosition());
			data.Depth = minDist - dist;
		//	data.Sign = 1.0f;

			return true;
		}

		static bool CollidePolygons(const Rigidbody2D& collider1, const Rigidbody2D& collider2, CollisionData2D& data)
		{
			data.Depth = std::numeric_limits<float>::max();
			data.Normal = glm::vec2();
		//	data.Sign = 1.0f;

			const ColliderVertices2D& vertices1 = collider1.GetTransformedVertices();
			const ColliderVertices2D& vertices2 = collider2.GetTransformedVertices();
			for (uint8_t i = 0; i < vertices1.length(); i++)
			{
				const glm::vec2& v1 = vertices1[i];
				const glm::vec2& v2 = vertices1[(i + 1) % vertices1.length()];

				const glm::vec2 edge = v2 - v1;

				const glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

				const ProjectionResult r1 = ProjectVertices(vertices1, axis);
				const ProjectionResult r2 = ProjectVertices(vertices2, axis);

				if (r1.Min >= r2.Max || r2.Min >= r1.Max)
					return false;

				float depth = glm::min(r1.Max - r2.Min, r2.Max - r1.Min);


				// faster but rn broken way to do it, maybe consider
				/*float depth = r1.Max - r2.Min;
				float sign = 1.0f;
				if (r2.Max - r1.Min < depth)
				{
					depth = r2.Max - r1.Min;
					sign = -1.0f;
				}*/

				if (depth < data.Depth)
				{
					data.Depth = depth;
					data.Normal = axis;
				//	data.Sign = 1.0f;
				}
			}

			for (uint8_t i = 0; i < vertices2.length(); i++)
			{
				const glm::vec2& v1 = vertices2[i];
				const glm::vec2& v2 = vertices2[(i + 1) % vertices2.length()];

				const glm::vec2 edge = v2 - v1;

				const glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

				const ProjectionResult r1 = ProjectVertices(vertices2, axis);
				const ProjectionResult r2 = ProjectVertices(vertices1, axis);

				if (r1.Min >= r2.Max || r2.Min >= r1.Max)
					return false;

				float depth = glm::min(r1.Max - r2.Min, r2.Max - r1.Min);

				/*float depth = r1.Max - r2.Min;
				float sign = -1.0f;
				if (r2.Max - r1.Min < depth)
				{
					depth = r2.Max - r1.Min;
					sign = 1.0f;
				}*/

				if (depth < data.Depth)
				{
					data.Depth = depth;
					data.Normal = axis;
				//	data.Sign = 1.0f;
				}
			}

			const glm::vec2 dir = collider2.GetPosition() - collider1.GetPosition();
			if (glm::dot(dir, data.Normal) < 0.0f)
				data.Normal = -data.Normal;

			return true;
		}

		static bool CollideCirclePolygon(const Rigidbody2D& circle, const Rigidbody2D& polygon, CollisionData2D& data)
		{
			data.Depth = std::numeric_limits<float>::max();
			data.Normal = glm::vec2();
		//	data.Sign = 1.0f;

			const ColliderVertices2D& vertices = polygon.GetTransformedVertices();
			for (uint8_t i = 0; i < vertices.length(); i++)
			{
				const glm::vec2& v1 = vertices[i];
				const glm::vec2& v2 = vertices[(i + 1) % vertices.length()];

				const glm::vec2 edge = v2 - v1;

				const glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

				const ProjectionResult r1 = ProjectVertices(vertices, axis);
				const ProjectionResult r2 = ProjectCircle(circle.GetPosition(), circle.GetRadius(), axis);

				if (r1.Min >= r2.Max || r2.Min >= r1.Max)
					return false;

				float depth = glm::min(r1.Max - r2.Min, r2.Max - r1.Min);

				/*float depth = r1.Max - r2.Min;
				float sign = 1.0f;
				if (r2.Max - r1.Min < depth)
				{
					depth = r2.Max - r1.Min;
					sign = -1.0f;
				}*/

				if (depth < data.Depth)
				{
					data.Depth = depth;
					data.Normal = axis;
				//	data.Sign = 1.0f;
				}
			}

			const int cpIndex = ClosestVertexToPoint(circle.GetPosition(), vertices);
			const glm::vec2 cp = vertices[cpIndex];

			const glm::vec2 axis = glm::normalize(cp - circle.GetPosition());

			const ProjectionResult r1 = ProjectVertices(vertices, axis);
			const ProjectionResult r2 = ProjectCircle(circle.GetPosition(), circle.GetRadius(), axis);

			if (r1.Min >= r2.Max || r2.Min >= r1.Max)
				return false;

			float depth = glm::min(r1.Max - r2.Min, r2.Max - r1.Min);

			/*float depth = r1.Max - r2.Min;
			float sign = 1.0f;
			if (r2.Max - r1.Min < depth)
			{
				depth = r2.Max - r1.Min;
				sign = -1.0f;
			}*/

			if (depth < data.Depth)
			{
				data.Depth = depth;
				data.Normal = axis;
			//	data.Sign = 1.0f;
			}

			const glm::vec2 dir = polygon.GetPosition() - circle.GetPosition();
			if (glm::dot(dir, data.Normal) < 0.0f)
				data.Normal = -data.Normal;

			return true;
		}




		static void FindCPCircles(const Rigidbody2D& circle1, const Rigidbody2D& circle2, CollisionManifold2D& manifold)
		{
			manifold.ContactCount = 1;

			const glm::vec2 dir = glm::normalize(circle2.GetPosition() - circle1.GetPosition());
			manifold.Contact1 = circle1.GetPosition() + dir * circle1.GetRadius();
		}

		static void FindCPCirclePolygon(const Rigidbody2D& circle, const Rigidbody2D& polygon, CollisionManifold2D& manifold)
		{
			manifold.ContactCount = 1;

			float minDist2 = std::numeric_limits<float>::max();
			const ColliderVertices2D& vertices = polygon.GetTransformedVertices();
			for (uint8_t i = 0; i < vertices.length(); i++)
			{
				const glm::vec2& v1 = vertices[i];
				const glm::vec2& v2 = vertices[(i + 1) % vertices.length()];

				glm::vec2 contact{};
				float dist2 = PointSegmentDist2(circle.GetPosition(), v1, v2, contact);

				if (dist2 < minDist2)
				{
					minDist2 = dist2;
					manifold.Contact1 = contact;
				}
			}
		}

		static void FindCPPolygons(const Rigidbody2D& polygon1, const Rigidbody2D& polygon2, CollisionManifold2D& manifold)
		{
			float minDist2 = std::numeric_limits<float>::max();
			const ColliderVertices2D& vertices1 = polygon1.GetTransformedVertices();
			const ColliderVertices2D& vertices2 = polygon2.GetTransformedVertices();
			for (uint8_t i = 0; i < vertices1.length(); i++)
			{
				const glm::vec2& p = vertices1[i];
				for (uint8_t j = 0; j < vertices2.length(); j++)
				{
					const glm::vec2& v1 = vertices2[j];
					const glm::vec2& v2 = vertices2[(j + 1) % vertices2.length()];

					glm::vec2 contact{};
					float dist2 = PointSegmentDist2(p, v1, v2, contact);

					if (glm::epsilonEqual(dist2, minDist2, c_HalfMilimeter))
					{
						if (glm::any(glm::epsilonNotEqual(contact, manifold.Contact1, c_HalfMilimeter)))
						{
							manifold.Contact2 = p;
							manifold.ContactCount = 2;
						}
					}
					else if (dist2 < minDist2)
					{
						minDist2 = dist2;
						manifold.Contact1 = contact;
						manifold.ContactCount = 1;
					}
				}
			}

			for (uint8_t i = 0; i < vertices2.length(); i++)
			{
				const glm::vec2& p = vertices2[i];
				for (uint8_t j = 0; j < vertices1.length(); j++)
				{
					const glm::vec2& v1 = vertices1[j];
					const glm::vec2& v2 = vertices1[(j + 1) % vertices1.length()];

					glm::vec2 contact{};
					float dist2 = PointSegmentDist2(p, v1, v2, contact);

					if (glm::epsilonEqual(dist2, minDist2, c_HalfMilimeter))
					{
						if (glm::any(glm::epsilonNotEqual(contact, manifold.Contact1, c_HalfMilimeter)))
						{
							manifold.Contact2 = p;
							manifold.ContactCount = 2;
						}
					}
					else if (dist2 < minDist2)
					{
						minDist2 = dist2;
						manifold.Contact1 = contact;
						manifold.ContactCount = 1;
					}
				}
			}
		}


	private:
		struct ProjectionResult
		{
			float Min = std::numeric_limits<float>::max();
			float Max = std::numeric_limits<float>::min();
		};

		static ProjectionResult ProjectVertices(const ColliderVertices2D& vertices, glm::vec2 axis)
		{
			ProjectionResult result;

			for (uint8_t i = 0; i < vertices.length(); i++)
			{
				const glm::vec2& vertex = vertices[i];
				const float proj = glm::dot(vertex, axis);

				if (proj < result.Min) result.Min = proj;
				if (proj > result.Max) result.Max = proj;
			}

			return result;
		}

		static ProjectionResult ProjectCircle(glm::vec2 center, float radius, glm::vec2 axis)
		{
			const glm::vec2 radiusOnAxis = axis * radius;

			const glm::vec2 p1 = center + radiusOnAxis;
			const glm::vec2 p2 = center - radiusOnAxis;

			ProjectionResult result{ glm::dot(p1, axis), glm::dot(p2, axis) };

			if (result.Min > result.Max)
				std::swap(result.Min, result.Max);

			return result;
		}

		static uint8_t ClosestVertexToPoint(glm::vec2 point, const ColliderVertices2D& vertices)
		{
			uint8_t index = -1;
			float minDist = std::numeric_limits<float>::max();
			for (uint8_t i = 0; i < vertices.length(); i++)
			{
				const float dist = glm::distance(point, vertices[i]);

				if (dist < minDist)
				{
					minDist = dist;
					index = i;
				}
			}

			return index;
		}


		static float PointSegmentDist2(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2& contact)
		{
			const glm::vec2 ab = b - a;
			const glm::vec2 ap = p - a;

			const float proj = glm::dot(ap, ab);
			const float abLen2 = glm::dot(ab, ab);
			const float d = proj / abLen2;

			if (d <= 0.0f)
				contact = a;
			else if (d >= 1.0f)
				contact = b;
			else contact = a + ab * d;

			const glm::vec2 diff = p - contact;
			return glm::dot(diff, diff);
		}
	};
}