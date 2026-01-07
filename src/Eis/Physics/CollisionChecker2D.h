#pragma once

#include <glm/gtc/epsilon.hpp>

#include "Eis/Physics/Objects/Colliders/Collider2D.h"
#include "Eis/Physics/Objects/Colliders/CircleCollider2D.h"
#include "Eis/Physics/Objects/Colliders/PolygonCollider2D.h"
#include "Eis/Physics/Objects/Rigidbody2D.h"


namespace Eis
{
	static constexpr float c_SmallDist = std::numeric_limits<float>::epsilon();

	struct CollisionData2D
	{
		glm::vec2 Normal = glm::vec2{};
		float Depth = std::numeric_limits<float>::max();
	//	float Sign = 1.0f;
	};

	struct CollisionManifold2D
	{
		CollisionManifold2D(Rigidbody2D* b1, Rigidbody2D* b2)
		{
			if (b2->GetCollider().GetType() == Collider2D::Type::CIRCLE
				&& b1->GetCollider().GetType() == Collider2D::Type::POLYGON)
				Body1 = b2, Body2 = b1;
			else
				Body1 = b1, Body2 = b2;
		}

		Rigidbody2D* Body1;
		Rigidbody2D* Body2;
		CollisionData2D Data{}; 
		glm::vec2 Contact1{}, Contact2{};
		uint8_t ContactCount = 0;
	};


	class CollisionChecker2D
	{
	public:
		static bool CheckBBIntersection(const Rigidbody2D& collider1, const Rigidbody2D& collider2)
		{
			const BBox2D& bb1 = collider1.GetBBox();
			const BBox2D& bb2 = collider2.GetBBox();

			if (bb1.TopRight.x <= bb2.BottomLeft.x || bb2.TopRight.x <= bb1.BottomLeft.x
			 || bb1.TopRight.y <= bb2.BottomLeft.y || bb2.TopRight.y <= bb1.BottomLeft.y)
				return false;

			return true;
		}

		static bool CheckCollision(CollisionManifold2D& manifold)
		{
			const Rigidbody2D& b1Collider = *manifold.Body1;
			const Rigidbody2D& b2Collider = *manifold.Body2;
			const Collider2D::Type& b1Type = b1Collider.GetCollider().GetType();
			const Collider2D::Type& b2Type = b2Collider.GetCollider().GetType();

			if (b1Type == Collider2D::Type::CIRCLE)
			{
				if (b2Type == Collider2D::Type::CIRCLE)
					return CollideCircles(b1Collider, b2Collider, manifold.Data);

				else if (b2Type == Collider2D::Type::POLYGON)
					return CollideCirclePolygon(b1Collider, b2Collider, manifold.Data);
			}
			else if (b1Type == Collider2D::Type::POLYGON)
			{
				if (b2Type == Collider2D::Type::POLYGON)
					return CollidePolygons(b1Collider, b2Collider, manifold.Data);

				else if (b2Type == Collider2D::Type::CIRCLE)
					return CollideCirclePolygon(b2Collider, b1Collider, manifold.Data);
			}

			EIS_CORE_ASSERT(false, "Collision type could not be determined!");
			return false;
		}

		static void FindContactPoints(CollisionManifold2D& manifold)
		{
			const Rigidbody2D& b1Collider = *manifold.Body1;
			const Rigidbody2D& b2Collider = *manifold.Body2;
			const Collider2D::Type& b1Type = b1Collider.GetCollider().GetType();
			const Collider2D::Type& b2Type = b2Collider.GetCollider().GetType();

			if (b1Type == Collider2D::Type::CIRCLE)
			{
				if (b2Type == Collider2D::Type::CIRCLE)
					return FindCPCircles(b1Collider, b2Collider, manifold);

				else if (b2Type == Collider2D::Type::POLYGON)
					return FindCPCirclePolygon(b1Collider, b2Collider, manifold);
			}
			else if (b1Type == Collider2D::Type::POLYGON)
			{
				if (b2Type == Collider2D::Type::POLYGON)
					return FindCPPolygons(b1Collider, b2Collider, manifold);

				else if (b2Type == Collider2D::Type::CIRCLE)
					return FindCPCirclePolygon(b2Collider, b1Collider, manifold);
			}

			EIS_CORE_ASSERT(false, "?");
		}


	private:
		static bool CollideCircles(const Rigidbody2D& b1, const Rigidbody2D& b2, CollisionData2D& data)
		{
			const float dist = glm::distance(b1.GetPosition(), b2.GetPosition());
			const float minDist = b1.GetCollider().As<CircleCollider2D>().GetRadius()
								+ b2.GetCollider().As<CircleCollider2D>().GetRadius();

			if (dist >= minDist)
				return false;

			data.Normal = glm::normalize(b2.GetPosition() - b1.GetPosition());
			data.Depth = minDist - dist;
		//	data.Sign = 1.0f;

			return true;
		}

		static bool CollidePolygons(const Rigidbody2D& polygon1,
									const Rigidbody2D& polygon2,
									CollisionData2D& data)
		{
			data.Depth = std::numeric_limits<float>::max();
			data.Normal = glm::vec2{};
		//	data.Sign = 1.0f;

			const PolygonCollider2D& collider1 = polygon1.GetCollider().As<PolygonCollider2D>();
			const PolygonCollider2D& collider2 = polygon2.GetCollider().As<PolygonCollider2D>();

			const auto& vertices1 = collider1.GetTransformedVertices(polygon1.GetPosition(), polygon1.GetRotation());
			const auto& vertices2 = collider2.GetTransformedVertices(polygon2.GetPosition(), polygon2.GetRotation());
			for (uint8_t i = 0; i < vertices1.size(); i++)
			{
				const glm::vec2& v1 = vertices1[i];
				const glm::vec2& v2 = vertices1[(i + 1) % vertices1.size()];

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

			for (uint8_t i = 0; i < vertices2.size(); i++)
			{
				const glm::vec2& v1 = vertices2[i];
				const glm::vec2& v2 = vertices2[(i + 1) % vertices2.size()];

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

			const glm::vec2 dir = polygon2.GetPosition() - polygon1.GetPosition();
			if (glm::dot(dir, data.Normal) < 0.0f)
				data.Normal = -data.Normal;

			return true;
		}

		static bool CollideCirclePolygon(const Rigidbody2D& circle,
										const Rigidbody2D& polygon,
										CollisionData2D& data)
		{
			data.Depth = std::numeric_limits<float>::max();
			data.Normal = glm::vec2{};
		//	data.Sign = 1.0f;

			const CircleCollider2D& circleCol = circle.GetCollider().As<CircleCollider2D>();
			const PolygonCollider2D& polyCol = polygon.GetCollider().As<PolygonCollider2D>();

			const auto& vertices = polyCol.GetTransformedVertices(polygon.GetPosition(), polygon.GetRotation());
			for (uint8_t i = 0; i < vertices.size(); i++)
			{
				const glm::vec2& v1 = vertices[i];
				const glm::vec2& v2 = vertices[(i + 1) % vertices.size()];

				const glm::vec2 edge = v2 - v1;

				const glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

				const ProjectionResult r1 = ProjectVertices(vertices, axis);
				const ProjectionResult r2 = ProjectCircle(circle.GetPosition(), circleCol.GetRadius(), axis);

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
			const ProjectionResult r2 = ProjectCircle(circle.GetPosition(), circleCol.GetRadius(), axis);

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




		static void FindCPCircles(const Rigidbody2D& circle1,
									const Rigidbody2D& circle2,
									CollisionManifold2D& manifold)
		{
			manifold.ContactCount = 1;

			const glm::vec2 dir = glm::normalize(circle2.GetPosition() - circle1.GetPosition());
			manifold.Contact1 = circle1.GetPosition() + dir * circle1.GetCollider().As<CircleCollider2D>().GetRadius();
		}

		static void FindCPCirclePolygon(const Rigidbody2D& circle,
										const Rigidbody2D& polygon,
										CollisionManifold2D& manifold)
		{
			manifold.ContactCount = 1;

			const CircleCollider2D& circleCol = circle.GetCollider().As<CircleCollider2D>();
			const PolygonCollider2D& polyCol = polygon.GetCollider().As<PolygonCollider2D>();

			float minDist2 = std::numeric_limits<float>::max();
			const auto& vertices = polyCol.GetTransformedVertices(polygon.GetPosition(), polygon.GetRotation());
			for (uint8_t i = 0; i < vertices.size(); i++)
			{
				const glm::vec2& v1 = vertices[i];
				const glm::vec2& v2 = vertices[(i + 1) % vertices.size()];

				glm::vec2 contact{};
				float dist2 = PointSegmentDist2(circle.GetPosition(), v1, v2, contact);

				if (dist2 < minDist2)
				{
					minDist2 = dist2;
					manifold.Contact1 = contact;
				}
			}
		}

		static void FindCPPolygons(const Rigidbody2D& polygon1,
									const Rigidbody2D& polygon2,
									CollisionManifold2D& manifold)
		{
			const PolygonCollider2D& collider1 = polygon1.GetCollider().As<PolygonCollider2D>();
			const PolygonCollider2D& collider2 = polygon2.GetCollider().As<PolygonCollider2D>();

			float minDist2 = std::numeric_limits<float>::max();
			const auto& vertices1 = collider1.GetTransformedVertices(polygon1.GetPosition(), polygon1.GetRotation());
			const auto& vertices2 = collider2.GetTransformedVertices(polygon2.GetPosition(), polygon2.GetRotation());
			for (uint8_t i = 0; i < vertices1.size(); i++)
			{
				const glm::vec2& p = vertices1[i];
				for (uint8_t j = 0; j < vertices2.size(); j++)
				{
					const glm::vec2& v1 = vertices2[j];
					const glm::vec2& v2 = vertices2[(j + 1) % vertices2.size()];

					glm::vec2 contact{};
					float dist2 = PointSegmentDist2(p, v1, v2, contact);

					if (glm::epsilonEqual(dist2, minDist2, c_SmallDist))
					{
						if (glm::any(glm::epsilonNotEqual(contact, manifold.Contact1, c_SmallDist)))
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

			for (uint8_t i = 0; i < vertices2.size(); i++)
			{
				const glm::vec2& p = vertices2[i];
				for (uint8_t j = 0; j < vertices1.size(); j++)
				{
					const glm::vec2& v1 = vertices1[j];
					const glm::vec2& v2 = vertices1[(j + 1) % vertices1.size()];

					glm::vec2 contact{};
					float dist2 = PointSegmentDist2(p, v1, v2, contact);

					if (glm::epsilonEqual(dist2, minDist2, c_SmallDist))
					{
						if (glm::any(glm::epsilonNotEqual(contact, manifold.Contact1, c_SmallDist)))
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

		static ProjectionResult ProjectVertices(const Vertices& vertices, glm::vec2 axis)
		{
			ProjectionResult result;

			for (uint8_t i = 0; i < vertices.size(); i++)
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

		static uint8_t ClosestVertexToPoint(glm::vec2 point, const Vertices& vertices)
		{
			uint8_t index = -1;
			float minDist = std::numeric_limits<float>::max();
			for (uint8_t i = 0; i < vertices.size(); i++)
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